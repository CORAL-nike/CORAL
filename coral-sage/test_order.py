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
    NumberField, var,
)

from ideals_utils import short_ideals, ideal_to_sage
from ideal2d import sample_ideal2d

from sage.misc.functional import isqrt
from sage.rings.integer import Integer

logger = getLogger("Coral")
logger_sh = StreamHandler()
formatter = Formatter("%(name)s [%(levelname)s] %(message)s")
logger_sh.setFormatter(formatter)
logger.addHandler(logger_sh)

proof.all(False)

class_number_500 = 30115877202646647376923175490783656066559648055235996072894708571193049262827
fact_class_number_500 = [101, 409, 3943, 17851, 5503889543116441, 1881879447649265588797081861911138024133873569131]
p_pegasis = 2**503 * 33 - 1



def sample_ideal_safe():
    """
    Sample an ideal in the class group with norm 2^e, ensuring that the
    parameters are suitable for PEGASIS.
    """
    p = p_pegasis

    K = NumberField(name="pi", polynomial=var("x") ** 2 + p)
    order = K.order_of_conductor(2)
    w = K.gens()[0]
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


def id_square(i, p, O):
    if i.is_one():
        return i
    i = i**2
    sp = isqrt(p) + 1
    n, I = short_ideals(i, i.norm(), 1000 * sp, p)[0]
    I = ideal_to_sage(I, O)
    return I


def id_mult(i1, i2, p, O):
    i = i1 * i2
    sp = isqrt(p) + 1
    n, I = short_ideals(i, i.norm(), 1000 * sp, p)[0]
    I = ideal_to_sage(I, O)
    return I


def square_and_multiply(i, exponent, p, O):
    result = Integer(1)  # identity ideal
    exponent = Integer(exponent)
    for bit in bin(exponent)[2:]:
        result = id_square(result, p, O)
        if bit == "1":
            result = id_mult(result, i, p, O)
    return result


def check_max_order(ideal, order, fact, p, O):
    # assuming there are no powers
    assert order == prod(fact)
    for s in set(fact):
        h_s = order / s
        out = square_and_multiply(ideal, h_s, p, O)
        if out.is_one():
            print(f"Error, factor {s} found one")
            return False
    out = square_and_multiply(ideal, order, p, O)
    if out.is_one():
        return True
    else:
        print("Input order is not correct")
        return False


def test_ideal_order():
    """
    Test that the sampled ideal has the correct order in the class group.
    """
    x_frak, ideal, O = sample_ideal_safe()
    x, y, q, e = ideal
    p = ideal.p

    from params import class_number_500 as class_number
    from params import fact_class_number_500 as fact

    if check_max_order(x_frak, class_number, fact, p, O):
        logger.info("Sampled ideal has the correct order in the class group.")
        return True
    else:
        logger.error("Sampled ideal does not have the correct order in the class group.")
        print(f"Ideal: {x_frak}, norm: {x_frak.norm()}")
        print(f"{q = }, {x = }, {y = }, {e = }, {p = }")
        return False




if __name__ == "__main__":
    logger.setLevel(logging.INFO)

    num_tests = 100
    print(f"Running {num_tests} tests for ideal order...")
    for _ in range(num_tests):
        test_ideal_order()
