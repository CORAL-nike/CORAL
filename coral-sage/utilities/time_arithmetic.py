#!/usr/bin/env python3

from argparse import ArgumentParser
from datetime import datetime, timedelta
from itertools import product

from sage.all import GF, is_prime, log, proof

proof.all(False)


def human(size):
    """Return a number in human readable form"""
    # size /= 1000
    for magnitude in ["", "K", "M", "G", "T"]:
        if abs(size) < 1000:
            return f"{round(size, 2)}{magnitude}"
        size /= 1000


def time_arithmetic(p, repeats):
    times = {
        "Fp_add": 0.0,
        "Fp_sub": 0.0,
        "Fp_neg": 0.0,
        "Fp_mul": 0.0,
        "Fp_div": 0.0,
        "Fp_inv": 0.0,
        "Fp_pow": 0.0,
        "Fp_squ": 0.0,
        "Fp2_add": 0.0,
        "Fp2_sub": 0.0,
        "Fp2_neg": 0.0,
        "Fp2_mul": 0.0,
        "Fp2_div": 0.0,
        "Fp2_inv": 0.0,
        "Fp2_pow": 0.0,
        "Fp2_squ": 0.0,
    }

    fpn_timing = datetime.now()
    print(f":: Gathering avg timings of Fpn operations (log(p) = {log(p, 2).n():.1f}, {human(repeats**2)} runs)")
    print()
    # indent (2) operation (3) colon (1) space (1) 9.1 format + 2 for ns -2 for Fp -1 for ? space (1) 9.1 format + 2 for ns -3 for Fp2 -1 for ?
    print("  " + 3 * " " + " " + " " + " " * (9 + 1 + 2 - 3) + "Fp" + " " + " " * (9 + 1 + 2 - 4) + "Fp2")

    Fp = GF(p)
    Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])

    # Addition
    for field in ["Fp", "Fp2"]:
        F = Fp if field == "Fp" else Fp2
        elts = [F.random_element() for _ in range(repeats)]

        start = datetime.now()
        for el, fl in product(elts, repeat=2):
            el + fl
        _t = (datetime.now() - start) / timedelta(microseconds=1) * 1000 / repeats**2
        times[f"{field}_add"] = _t

    print(f"  Add: {times['Fp_add']:>9.1f}ns {times['Fp2_add']:>9.1f}ns")

    # Subtraction
    for field in ["Fp", "Fp2"]:
        F = Fp if field == "Fp" else Fp2
        elts = [F.random_element() for _ in range(repeats)]

        start = datetime.now()
        for el, fl in product(elts, repeat=2):
            el - fl
        _t = (datetime.now() - start) / timedelta(microseconds=1) * 1000 / repeats**2
        times[f"{field}_sub"] = _t

    print(f"  Sub: {times['Fp_sub']:>9.1f}ns {times['Fp2_sub']:>9.1f}ns")

    # Negation
    for field in ["Fp", "Fp2"]:
        F = Fp if field == "Fp" else Fp2
        elts = [F.random_element() for _ in range(repeats)]

        start = datetime.now()
        for el, fl in product(elts, repeat=2):
            -fl
        _t = (datetime.now() - start) / timedelta(microseconds=1) * 1000 / repeats**2
        times[f"{field}_neg"] = _t

    print(f"  Neg: {times['Fp_neg']:>9.1f}ns {times['Fp2_neg']:>9.1f}ns")

    # Multiplication
    for field in ["Fp", "Fp2"]:
        F = Fp if field == "Fp" else Fp2
        elts = [F.random_element() for _ in range(repeats)]

        start = datetime.now()
        for el, fl in product(elts, repeat=2):
            el * fl
        _t = (datetime.now() - start) / timedelta(microseconds=1) * 1000 / repeats**2
        times[f"{field}_mul"] = _t

    print(f"  Mul: {times['Fp_mul']:>9.1f}ns {times['Fp2_mul']:>9.1f}ns")

    # Division
    for field in ["Fp", "Fp2"]:
        F = Fp if field == "Fp" else Fp2
        elts = [F.random_element() for _ in range(repeats)]

        start = datetime.now()
        for el, fl in product(elts, repeat=2):
            el / fl
        _t = (datetime.now() - start) / timedelta(microseconds=1) * 1000 / repeats**2
        times[f"{field}_div"] = _t

    print(f"  Div: {times['Fp_div']:>9.1f}ns {times['Fp2_div']:>9.1f}ns")

    # Inversion
    for field in ["Fp", "Fp2"]:
        F = Fp if field == "Fp" else Fp2
        elts = [F.random_element() for _ in range(repeats)]

        start = datetime.now()
        for el, fl in product(elts, repeat=2):
            1 / fl
        _t = (datetime.now() - start) / timedelta(microseconds=1) * 1000 / repeats**2
        times[f"{field}_inv"] = _t

    print(f"  Inv: {times['Fp_inv']:>9.1f}ns {times['Fp2_inv']:>9.1f}ns")

    # Squaring
    for field in ["Fp", "Fp2"]:
        F = Fp if field == "Fp" else Fp2
        elts = [F.random_element() for _ in range(repeats)]

        start = datetime.now()
        for el, fl in product(elts, repeat=2):
            el**2
        _t = (datetime.now() - start) / timedelta(microseconds=1) * 1000 / repeats**2
        times[f"{field}_squ"] = _t

    print(f"  Squ: {times['Fp_squ']:>9.1f}ns {times['Fp2_squ']:>9.1f}ns")

    print()
    print(f"Done. Took {(datetime.now() - fpn_timing) / timedelta(seconds=1):.1f}s")

    return times


if __name__ == "__main__":
    parser = ArgumentParser(prog="time_arithmetic", description="", epilog="")
    parser.add_argument(
        "prime", type=int, nargs=2, help="Prime shape given as two integers `c, f` s.t. p = c * 2**f - 1"
    )
    args = parser.parse_args()

    c, f = args.prime
    p = c * 2**f - 1

    if not is_prime(p):
        raise AssertionError(f"p = {c} * 2**{f} - 1 is not prime")

    time_arithmetic(p, 100)
