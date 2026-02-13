#!/usr/bin/env python3

from sage.calculus.var import var
from sage.rings.finite_rings.finite_field_constructor import GF
from sage.schemes.elliptic_curves.constructor import EllipticCurve

from libs.pegasis.xonly import MontgomeryA, xPoint


def eval_frob(P):
    E = P.curve()

    if __debug__:
        p = E.base_field().characteristic()
        x, y = P.xy()
        # Check that .conjugate() does what we expect
        # It apparently does not do exponentiation
        assert x**p == x.conjugate()
        assert y**p == y.conjugate()

    return E([c.conjugate() for c in P.xy()])


def find_Ts(E, only_T0=False):
    r"""
    Taken from PEGASIS
    Given a curve E, finds and marks the non-trivial
    2-torsion points according to Lemma D.1
    """
    X = E.base_field()["X"].gens()[0]
    f = X**2 + MontgomeryA(E) * X + 1
    lam1, lam2 = f.roots(multiplicities=False)

    R1 = E(lam1, 0)
    R2 = E(lam2, 0)
    R3 = E(0, 0)

    # Find T0
    Rs = [R1, R2]
    for T in Rs:
        if T.tate_pairing(T, 2, 1) != 1:
            T0 = T
            Rs.remove(T)
            break

    assert T0
    if only_T0:
        return T0

    assert T0.tate_pairing(T0, 2, 1) == -1
    Rs.append(R3)
    for T in Rs:
        if T.tate_pairing(T0, 2, 1) == 1:
            Tm1 = T
            Rs.remove(T)
            break

    assert Tm1
    T1 = Rs[0]
    assert T1.tate_pairing(T0, 2, 1) != 1

    return T0, Tm1, T1


def TwoTorsBasis(E, e):
    r"""
    Taken from PEGASIS
    Fast sampling of a basis P, Q of E[2**e], such that x(P) and x(Q) are both defined over Fp
    Input:
        - E: Elliptic curve over Fp
        - e: Exponent
    Output:
        - P, Q: Basis of E[2**e] so that P is in E(Fp), and Q is in E^t(Fp) for an Fp-twist of E.
    """
    p = E.base_field().characteristic()

    assert E.base_field() == GF(p)

    T0, T1, Tm1 = find_Ts(E)

    F = E.base_field()
    X = F["X"].gens()[0]
    f = X**2 + MontgomeryA(E) * X + 1

    xT0 = T0.x()
    xP = xT0 + F.random_element() ** 2

    Tm1x = Tm1.x()
    while not (f(xP) * xP).is_square() or (xP - Tm1x).is_square():
        xP = xT0 + F.random_element() ** 2

    xQ = xT0 - F.random_element() ** 2

    T1x = T1.x()
    while (f(xQ) * xQ).is_square() or not ((xQ - T1x).is_square()):
        xQ = xT0 - F.random_element() ** 2

    P = xPoint(xP, E)
    Q = xPoint(xQ, E)

    assert (p + 1) % 2 ** (e + 1) == 0

    cofactor = (p + 1) // 2 ** (e + 1)
    P = P.xMUL(cofactor)
    Q = Q.xMUL(cofactor)

    assert P.xMUL(2 ** (e - 1))
    assert not P.xMUL(2**e)
    assert Q.xMUL(2 ** (e - 1))
    assert not Q.xMUL(2**e)
    assert Q.xMUL(2 ** (e - 1)) != P.xMUL(2 ** (e - 1))

    return P, Q


def fp_basis(E, e, xonly=False):
    Et = EllipticCurve([0, -MontgomeryA(E), 0, 1, 0])

    P, Q = TwoTorsBasis(E, e)

    if xonly:
        return P, Q

    P, Q = E.lift_x(P.X), Et.lift_x(-Q.X)

    if __debug__:
        # These calls are very expensive (25ms)
        P.set_order(multiple=2**e)
        Q.set_order(multiple=2**e)

        assert P.order() == 2**e
        assert Q.order() == 2**e

    return P, Q


def twist(E):
    montgomery_coeffs = E.montgomery_model().a_invariants()
    A = montgomery_coeffs[1]
    p = E.base_field().characteristic()
    Etwist = EllipticCurve(GF(p), [0, -A, 0, 1, 0])

    return Etwist


def twisted_base(E, e):
    p = E.base_field().characteristic()
    while True:
        P = E.random_point()
        P = ((p + 1) // 2**e) * P
        P.set_order(multiple=2**e)

        if P.order() == 2**e:
            break

    EFp2 = E.change_ring(GF((p, 2), name="i", modulus=var("x") ** 2 + 1))
    Et = twist(E)

    PP = EFp2(P.xy())
    PP.set_order(multiple=2**e)

    while True:
        Q = Et.random_point()
        Q = ((p + 1) // 2**e) * Q
        Q.set_order(multiple=2**e)

        if Q.order() == 2**e:
            QQ = EFp2(Q.xy())
            QQ.set_order(multiple=2**e)

            wp = P.weil_pairing(Q, 2**e)

            if wp.multiplicative_order() == 2**e:
                break

    assert wp ** (2**e) == 1
    assert wp ** (2 ** (e - 1)) != 1
    assert PP.order() == 2**e
    assert QQ.order() == 2**e

    return P, Q, PP, QQ
