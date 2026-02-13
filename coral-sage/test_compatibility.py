#!/usr/bin/env python 3

import logging
from logging import Formatter, StreamHandler, getLogger

from sage.all import (
    Integer,
    ceil,
    isqrt,
    log,
    prod,
    proof,
    valuation,
)

from coral import eval_ideal, is_montgomery
from libs.pegasis.ideals import ideal_to_sage, short_ideals
from libs.pegasis.pegasis import PEGASIS
from libs.pegasis.uv_params import UV_params
from libs.two_isogenies.utilities.strategy import optimised_strategy
from ideal2d import sample_ideal2d

logger = getLogger("Coral")
logger_sh = StreamHandler()
formatter = Formatter("%(name)s [%(levelname)s] %(message)s")
logger_sh.setFormatter(formatter)
logger.addHandler(logger_sh)

proof.all(False)


def sample_ideal_safe():
    """
    Sample an ideal in the class group with norm 2^e, ensuring that the
    parameters are suitable for PEGASIS.
    """
    params = UV_params(500)  # 1000 | 1500 | 2000 | 4000
    p = params.p
    order = params.order
    w = params.w
    e_max = valuation(p + 1, 2)
    # e = e_max - 3
    e = 128 + e_max // 2 + log(log(p), 2.0)
    e = ceil(e)
    e = Integer(e)
    ideal = sample_ideal2d(p, e)
    q, x, y, e = ideal

    a = 2 ** (e - 1) - q
    assert 2 ** (e - 1) + a == 2**e - q

    assert (ideal.to_sage_ideal(order, w) * ideal.conjugate().to_sage_ideal(order, w)).is_principal()

    x_frak = ideal.to_sage_ideal(order, w)
    y_frak = order * (2**e - q) + order * (x + w * y)
    assert (y_frak * x_frak).is_principal()

    theta = x + w * y
    assert x**2 + y**2 * p == theta.norm() == q * (2**e - q)
    return x_frak, ideal, order




def test_pepgasis_compatible():
    """
    Test that the action of PEGASIS and the ideal evaluation give the same
    results on a random.
    """
    EGA = PEGASIS(500)  # 1000 | 1500 | 2000 | 4000
    E = EGA.E_start
    assert is_montgomery(E), "Curve is not Montgomery"

    x_frak, ideal, _ = sample_ideal_safe()
    e = ideal.e

    logger.info("Precomputing strategies...")
    strategies = {n: [n - 1] + optimised_strategy(n - 1) for n in range(e - 16, e)}

    e = ideal.e

    E_x = eval_ideal(E, ideal, strategies, C_friendly=False, montgomery=False)
    E_y = eval_ideal(E, ideal.conjugate(), strategies, C_friendly=False, montgomery=False)

    # Need conjugate to be compatible
    E_pegasis = EGA.action(EGA.E_start, x_frak.conjugate())

    print(E_pegasis)
    print(E_x)
    print(E_y)

    if E_x == E_pegasis:
        logger.info("PEGASIS and CORAL give the _identical_ result.")
        return True
    elif E_y == E_pegasis:
        logger.info("[Need conjugate] PEGASIS and CORAL give the _identical_ result.")
        return True
    elif E_x.is_isomorphic(E_pegasis):
        logger.info("PEGASIS and CORAL give _isomorphic_ results.")
        return True
    elif E_y.is_isomorphic(E_pegasis):
        logger.info("[Need conjugate] PEGASIS and ideal evaluation give _isomorphic_ results.")
        return True
    else:
        logger.error("PEGASIS and ideal evaluation give different results.")
        return False


if __name__ == "__main__":
    logger.setLevel(logging.INFO)

    num_tests = 10
    print(f"Running {num_tests} tests for PEGASIS compatibility...")
    for _ in range(num_tests):
        test_pepgasis_compatible()
