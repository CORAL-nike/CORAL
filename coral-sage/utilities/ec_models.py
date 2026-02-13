#!/usr/bin/env python3

from time import perf_counter_ns

from sage.all import GF, EllipticCurve, PolynomialRing


def montgomery_fp(E):
    start = perf_counter_ns()
    Fp = GF(E.base_field().characteristic())
    xs = Fp["xs"].gen()
    f = E.j_invariant() * (xs - 4) - 256 * (xs - 3) ** 3
    As = f.roots(Fp, multiplicities=False)
    assert len(As) == 3
    As = [pm1 * A for A in As for pm1 in [-1, 1]]
    assert len(As) == 6
    A = min(As)
    print(f"Normalising montgomery coef took: {(perf_counter_ns() - start) / 10**6}ms")
    return A


def weierstrass_to_montgomery(A, B):
    # Ref: https://eprint.iacr.org/2017/212 Sec 2.4
    # E: y^2 = x^3 + Ax + B -> E: y^2 = x(x^2 + Ax + 1)
    # Concrete implementation is taken from Sage
    # Where we enfore the twisting factor B = 1

    P = PolynomialRing(A.parent(), "x")
    # r^3 + Ar + B = 0, i.e. the x-coordinate of a 2-torsion point
    # Slight problem in C: we probably want to avoid root-finding
    # (Although this is not difficult to implement,
    # and we already have an implementation from concretedg project)
    # There are lots of pairing tricks to keep track of 2-torsion points,
    # that do not involve root finding and are already implemented in C
    r = min(P([B, A, 0, 1]).roots(multiplicities=False))
    s = (3 * r**2 + A).sqrt()

    assert s.is_square()

    _A = 3 * r / s
    _A = min([_A, -_A])

    assert EllipticCurve([0, _A, 0, 1, 0]).isomorphism_to(EllipticCurve([A, B]))

    return _A
