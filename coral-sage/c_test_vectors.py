#!/usr/bin/env python3

import logging
from argparse import ArgumentParser
from logging import Formatter, StreamHandler, getLogger
from math import ceil
from sys import stdout

from sage.all import EllipticCurve, GF, is_prime, proof

from coral import eval_ideal, is_correct_twist
from ideal2d import sample_ideal2d
from params import params
from precompute import load_strategies
from utilities.random_curve import random_maximal_fp_curve
from utilities.volcano import go_to_surface

logger = getLogger("Coral")
logger_sh = StreamHandler()
formatter = Formatter("%(name)s [%(levelname)s] %(message)s")
logger_sh.setFormatter(formatter)
logger.addHandler(logger_sh)
logger.setLevel(logging.INFO)


proof.all(False)


def print_curve(E, file=stdout):
    try:
        print(f"{int(E.a2()[0]):#0{p_width}x} {int(E.a2()[1]):#0{p_width}x}", file=file)
    except TypeError:
        print(f"{int(E.a2()):#0{p_width}x}", file=file)


def str_hex(x):
    try:
        return f"{int(x):#0{p_width}x}"
    except ValueError:
        return f"{int(x[0]):#0{p_width}x} + i*{int(x[1]):#0{p_width}x}"


def test_action(p, e, e128, e256, emax):
    logger.info("Pipe the output of this to `test_nike_action.c")
    strategies = load_strategies(logger, f"precomputed/precomputed_strategies.{args.prime:0>4}.txt", e128, e256, emax)

    ideal = sample_ideal2d(p, e)

    E = random_maximal_fp_curve(p).montgomery_model()
    E = go_to_surface(E)

    if not is_correct_twist(E):
        E = EllipticCurve(E.base_ring(), [0, -E.a2(), 0, 1, 0])

    E_ideal = eval_ideal(E, ideal, strategies)

    print(f"{ideal.p} {ideal.q} {ideal.x} {ideal.y} {ideal.e}")
    print_curve(E)
    print_curve(E_ideal)


if __name__ == "__main__":
    parser = ArgumentParser(prog="Coral", description="", epilog="")

    parser.add_argument("-p", "--prime", type=int, default=500, choices=params.keys())

    args = parser.parse_args()

    f, c, e128, e256, emax = params[args.prime]

    p = c * 2**f - 1
    p_width = ceil(p.bit_length() / 64) * 16 + 2
    assert is_prime(p)
    Fp = GF(p)
    Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])

    e = e128

    test_action(p, e, e128, e256, emax)
