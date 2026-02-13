#!/usr/bin/env python3

from time import perf_counter_ns

from sage.all import factor

from ideal2d import Ideal2d, sample_ideal2d
from libs.two_isogenies.utilities.strategy import optimised_strategy
from utilities.timingtable import TimingTable


def load_strategies(logger, filepath, e128, e256, emax):
    strategies = {}

    try:
        with open(filepath) as file:
            lines = file.readlines()[1:]
        logger.info(":: Parsed precomputed strategies for this prime")

    except FileNotFoundError:
        logger.warning(f":: Cannot find file `{filepath}`")
        start = perf_counter_ns()

        names = []
        es = []

        if e128 is not None:
            names += ["Aggressive1"]
            es += [e128]

        if e256 is not None:
            names += ["Aggressive2"]
            es += [e256]

        names += ["Conservative"]
        es += [emax]

        for regime, e in zip(names, es, strict=True):
            logger.warning(f"=> Precomputing {regime} strategies...")
            for n in range(e - 16, e):
                strategies[n] = optimised_strategy(n)

        logger.warning(f"Precomputing strategies...done (Took {(perf_counter_ns() - start) / 10**9:.3f} s).")
        logger.info(f"Writing to {filepath}")

        with open(filepath, "w") as file:
            file.write("# n [a_1, a_2, ..., a_n]\n")
            for n, strategy in strategies.items():
                file.write(f"{n} {' '.join([str(_) for _ in strategy])}\n")

        logger.info("Done.")

        with open(filepath) as file:
            lines = file.readlines()[1:]
        logger.info(":: Parsed precomputed strategies for this prime")

    strategies = {}
    for line in lines:
        n, *strategy = [int(_) for _ in line.split()]
        strategies[n] = strategy

    return strategies


def get_strategy(logger, strategies, n):
    strategy = strategies.get(n, None)
    if strategy is None:
        logger.warning("Strategy not precomputed. Precomputing now...")
        strategy_precomp = perf_counter_ns()
        strategy = [n - 1] + optimised_strategy(n - 1)
        logger.warning(f"Computing the strategy took {(perf_counter_ns() - strategy_precomp) / 10**6:.1f} ms")
    return strategy


def load_ideals(logger, filepath, p, e128, e256, emax):
    precomputed_ideals = {}

    try:
        with open(filepath) as file:
            lines = file.readlines()[1:]
        logger.info(":: Parsed precomputed ideals for this prime")

    except FileNotFoundError:
        logger.warning(f":: Cannot find file `{filepath}`")
        precomputed_ideals = {}
        with open(filepath, "w") as file:
            file.write("# p q x y e\n")

        names = []
        es = []
        if e128 is not None:
            names += ["Aggressive1"]
            es += [e128]

        if e256 is not None:
            names += ["Aggressive2"]
            es += [e256]

        names += ["Conservative"]
        es += [emax]

        for regime, e in zip(names, es, strict=True):
            norm_stats = TimingTable(precision="ms", round=3)
            for _ in range(100):
                logger.warning(f":: Precomputing {regime} {_ + 1}/100 (p + 1 = {factor(p + 1)})")
                ideal, norm_stats = sample_ideal2d(p, e, stats=norm_stats)
                logger.info(f"\n{norm_stats}")
                with open(filepath, "a") as file:
                    file.write(f"{ideal.p} {ideal.q} {ideal.x} {ideal.y} {ideal.e}\n")

                precomputed_ideals[(p, e)] = precomputed_ideals.get((p, e), []) + [ideal]

        logger.info("Done.")

        with open(filepath) as file:
            lines = file.readlines()[1:]

        logger.warning(":: Parsed precomputed strategies for this prime")

    for line in lines:
        p, q, x, y, e = [int(_) for _ in line.split()]
        # TwoDimIdeal implicitly verifies the values
        precomputed_ideals[(p, e)] = precomputed_ideals.get((p, e), []) + [Ideal2d(q=q, x=x, y=y, e=e, p=p)]

    return precomputed_ideals
