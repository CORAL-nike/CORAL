#!/usr/bin/env python3

from argparse import ArgumentParser
from logging import DEBUG, INFO, Formatter, StreamHandler, getLogger

from sage.all import (
    GF,
    EllipticCurve,
    Integer,
    is_prime,
    is_square,
    log,
    proof,
    valuation,
)

from ideal2d import sample_ideal2d
from libs.two_isogenies.theta_isogenies.product_isogeny import EllipticProductIsogeny
from libs.two_isogenies.utilities.discrete_log import weil_pairing_pari
from params import params, e_params
from precompute import get_strategy, load_ideals, load_strategies
from prepare_hd_kernel import prepare_hd_kernel
from utilities.basis_sampling import fp_basis
from utilities.table import jointable, printtable
from utilities.timingtable import TimingTable
from utilities.volcano import go_to_surface, on_surface

logger = getLogger("Coral")
logger_sh = StreamHandler()
formatter = Formatter("%(name)s [%(levelname)s] %(message)s")
logger_sh.setFormatter(formatter)
logger.addHandler(logger_sh)


__could_import_ffa_counter = False
try:
    from sage.rings.finite_rings.ffa_counter import FFA_counter

    __could_import_ffa_counter = True
except (ModuleNotFoundError, ImportError):
    logger.warning("WARNING: Sage not patched for counting finite field arithmetic")

    def FFA_counter():
        return 0


proof.all(False)


def weil_pairing_power_two(P, Q, e):
    return weil_pairing_pari(P, Q, 2**e)


def is_correct_twist(E):
    return is_square(E.a2() + 2) and is_square(E.a2() - 2)


def is_montgomery(E):
    a1, _, a3, a4, a6 = E.a_invariants()
    return a1 == a3 == a6 and a4 == 1


def eval_ideal(E, ideal, strategies, stats=None, C_friendly=True, montgomery=False):
    assert E.j_invariant() != E.base_field()(1728)
    assert is_montgomery(E)
    assert is_correct_twist(E)

    # We know that E is montgomery
    Et = EllipticCurve([0, -E.a2(), 0, 1, 0])

    if stats is not None:
        stats.init()

    p = E.base_ring().characteristic()

    assert valuation(p + 1, 2) >= ideal.e + 2

    P, Q = fp_basis(E, ideal.e + 2, xonly=False)

    assert P in E
    assert Q in Et

    if stats is not None:
        stats.update("Basis sampling")

    strategy = get_strategy(logger, strategies, ideal.e - (ideal.v + 1))

    kernel, gluing_kernel, stats = prepare_hd_kernel(
        E, P, Q, ideal, strategy, stats=stats, C_friendly=C_friendly, montgomery=montgomery
    )

    Phi = EllipticProductIsogeny(kernel, ideal.e - (ideal.v + 1), coerce=gluing_kernel, strategy=strategy)

    if stats is not None:
        stats.update("Evaluating 2d isogeny")

    E_ideal, _ = Phi.codomain()
    # This is apparently the consistent choice, according to how the two-library implements things
    E_ideal = EllipticCurve([0, -E_ideal.a2(), 0, 1, 0])

    if __debug__ and not is_correct_twist(E_ideal):
        logger.warning("Had to flip")
        E_ideal = EllipticCurve([0, -E_ideal.a2(), 0, 1, 0])

    if stats is not None:
        stats.update("Getting right output curve")

    # Outside of timing, this will eventually not be there
    # Will trigger, even if asserts are off
    if not is_correct_twist(E_ideal):
        raise AssertionError("Wrong curve")

    if stats is not None:
        return E_ideal, stats

    return E_ideal


def Fp2_mul():
    return FFA_counter().Fp2_mul


def Fp2_inv():
    return FFA_counter().Fp2_inv


def Fp2_div():
    return FFA_counter().Fp2_div


def Fp_mul():
    return FFA_counter().Fp_mul


def Fp_squ():
    return FFA_counter().Fp_squ


def Fp_inv():
    return FFA_counter().Fp_inv


def Fp_div():
    return FFA_counter().Fp_div


if __name__ == "__main__":
    parser = ArgumentParser(prog="Coral", description="", epilog="")

    # fmt: off
    parser.add_argument("-p", "--prime", type=int, default=500, choices=params.keys())
    parser.add_argument("-r", "--reps", type=int, default=1)
    parser.add_argument("-n", "--onlynormeq", action="store_true")
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument("-S", "--sage", action="store_true", help="Use sage `.isogeny()` instead of C-friendly impl")
    parser.add_argument("-i", "--precomputed-ideals", action="store_true", help="Deterministically use cached ideals.")
    parser.add_argument("-m", "--montgomery", action="store_true", help="Use montgomery form (implies `--sage`)")
    # fmt: on

    args = parser.parse_args()

    if args.montgomery:
        args.sage = True

    if args.verbose:
        logger.setLevel(DEBUG)
    else:
        logger.setLevel(INFO)

    f, c, e128, e256, emax = params[args.prime]
    p = c * 2**f - 1
    assert is_prime(p)
    Fp = GF(p)

    E0 = EllipticCurve(Fp, [0, 0, 0, 1, 0])
    E0 = go_to_surface(E0)

    # Choose twist class of the curve
    if not is_correct_twist(E0):
        E0 = EllipticCurve([0, -E0.a2(), 0, 1, 0])

    assert on_surface(E0)
    assert E0.j_invariant() != GF(p)(1728)

    ff_initial = FFA_counter()

    e = e128

    if args.precomputed_ideals:
        precomputed_ideals = load_ideals(logger, f"precomputed/precomputed_ideals.{args.prime:0>4}.txt", p, e128, e256, emax)
        if len(precomputed_ideals[(p, e)]) < args.reps:
            logger.warning(":: [WARNING] Less precomputed ideals than iterations. Will re-use ideals")

    if not args.onlynormeq:
        strategies = load_strategies(logger, f"precomputed/precomputed_strategies.{args.prime:0>4}.txt", e128, e256, emax)

    if __could_import_ffa_counter:
        isog_stats = TimingTable(
            precision="ms",
            round=3,
            counters=[Fp_mul, Fp_squ, Fp_inv, Fp_div, Fp2_mul, Fp2_inv, Fp2_inv],
            counter_names=["FpMul", "Fp_squ", "FpInv", "FpDiv", "Fp2Mul", "Fp2Inv", "Fp2Div"],
        )
    else:
        isog_stats = TimingTable(precision="ms", round=3)

    norm_stats = TimingTable(precision="ms", round=3)

    for iteration in range(args.reps):
        if args.precomputed_ideals:
            # KeyGen -- party A
            seckey_A = precomputed_ideals[(p, e)][(2 * iteration) % len(precomputed_ideals[(p, e)])]
            # KeyGen -- party B
            seckey_B = precomputed_ideals[(p, e)][(2 * iteration + 1) % len(precomputed_ideals[(p, e)])]
        else:
            # KeyGen -- party A
            seckey_A, norm_stats = sample_ideal2d(p, e, stats=norm_stats)
            # KeyGen -- party B
            seckey_B, norm_stats = sample_ideal2d(p, e, stats=norm_stats)

        if not args.onlynormeq:
            # KeyGen -- party A

            EA, isog_stats = eval_ideal(
                E0, seckey_A, strategies, stats=isog_stats, C_friendly=not args.sage, montgomery=args.montgomery
            )

            # KeyGen -- party B
            EB, isog_stats = eval_ideal(
                E0, seckey_B, strategies, stats=isog_stats, C_friendly=not args.sage, montgomery=args.montgomery
            )

            # Shared secret -- party A
            EAB, isog_stats = eval_ideal(
                EB, seckey_A, strategies, stats=isog_stats, C_friendly=not args.sage, montgomery=args.montgomery
            )

            # Shared secret -- party B
            EBA, isog_stats = eval_ideal(
                EA, seckey_B, strategies, stats=isog_stats, C_friendly=not args.sage, montgomery=args.montgomery
            )

            if args.precomputed_ideals:
                print(f":: Shared secret {EBA.a2()}")

            # raise AssertionError will still call when assertions are turned off
            if not EBA == EAB:
                raise AssertionError("Key exchange failed")

        print()
        print(f"log(p) = {Integer(p).nbits()}")
        print(f"Iteration {iteration + 1}/{args.reps}")

        if args.precomputed_ideals:
            print("Using pre-computed norm-equation solutions")
            printtable(isog_stats.table())
        else:
            printtable(jointable(norm_stats.table(), isog_stats.table()))
