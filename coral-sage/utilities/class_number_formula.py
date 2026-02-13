#!/usr/bin/env python3
# Written by Lorenz Panny

from sage.arith.misc import fundamental_discriminant, kronecker_symbol, primes
from sage.misc.functional import round, sqrt
from sage.rings.integer_ring import ZZ
from sage.rings.real_mpfr import RealField
from sage.symbolic.constants import pi


def approx_class_number(disc, bound=10**6):
    """
    Approximation of the class number of ℚ(√disc)
    using the analytic class number formula.
    """
    disc = ZZ(disc)
    if disc >= 0:
        raise NotImplementedError("only imaginary-quadratic fields supported")
    if disc != fundamental_discriminant(disc):
        raise NotImplementedError("only fundamental discriminants supported")
    w = 6 if disc == -3 else 4 if disc == -4 else 2
    RR = RealField(disc.bit_length() + 55)
    L = RR(1)
    for ell in primes(bound):
        L *= ell / (ell - kronecker_symbol(disc, ell))
    return round(w * sqrt(abs(disc)) * L / (2 * pi))
