#!/usr/bin/env python3

# Compute common EC operations without using Sage, useful for porting to C
# Sage modules are exclusively used for assertions

from random import choice

from sage.all import GF, EllipticCurve, PolynomialRing

from utilities.random_curve import random_maximal_curve

# Invariants {{{{{


def j_invariant_sw(E):
    """Compute j-invariant of E: y^2 = x^3 + (a/c)x + (b/c)"""
    a, b, c = E
    a, b = a / c, b / c
    j = 1728 * a**3 / (a**3 - b**2)
    assert j == EllipticCurve([E.a, E.b]).j_invariant()
    return j


def j_invariant_m(E):
    """Compute j-invariant of E: y^2 = x (x^2 + (A / C)x + 1)"""
    A, C = E
    A = A / C
    j = 256 * (A**2 - 3) ** 3 / (A**2 - 4)
    assert j == EllipticCurve([0, A, 0, 1, 0]).j_invariant()
    return j


# }}}}}


# Doubling {{{{{


def double_sw_x(E, Px):
    """Double x-point `Px` on the curve E: y^2 = x^3 + ax + b"""
    # Cost: 9A/S + 2S + 3M + 1I
    a, b = E

    Px_pow2 = Px**2
    Px_pow3 = Px * Px_pow2
    y_pow2 = Px_pow3 + a * Px + b
    y_pow2_x2 = y_pow2 + y_pow2
    m2 = (Px_pow2 + Px_pow2 + Px_pow2 + a) ** 2
    m2 = m2 / (y_pow2_x2 + y_pow2_x2)
    Qx = m2 - Px - Px

    if __debug__:
        p = a.parent().characteristic()
        assert p % 4 == 3
        Fp2 = GF((p, 2), name="i", modulus=[1, 0, 1])
        P = EllipticCurve(Fp2, [a, b]).lift_x(Px)
        T = EllipticCurve(Fp2, [a, b]).lift_x(Qx)
        assert 2 * P in [-T, T]

    return Qx


def double_sw_xz(E, C_pow2, C_pow3, Pxz):
    """Double xz-point `Pxz` on the curve E: y^2 = x^3 + (a/c^2)x + (b/c^3)"""
    # We demand that the curve constants (c_pow2, c_pow3) are pre-computed,
    # because it is often the case that multiple points need to be doubled on
    # the same curve

    A, B, C = E

    # Cost: 9A/S + 3S + 13M
    Px, Pz = Pxz

    # Rrojective implementation of the usual (affine) formula
    # x = (3x^2 + a)^2 / 4 / (x^3 + ax + b) - 2x
    # Must replace x <- x/z, a <- a/c^2, b <- b/c^3

    Px_pow2 = Px**2
    Px_pow3 = Px * Px_pow2
    Pz_pow2 = Pz**2
    Pz_pow3 = Pz_pow2 * Pz

    m2_n = Px_pow2 * C_pow2
    m2_n = m2_n + m2_n + m2_n
    m2_n = m2_n + A * Pz_pow2
    m2_n = m2_n**2

    m2_d = Px_pow3 * C_pow3 + C * A * Px * Pz_pow2 + B * Pz_pow3
    m2_d = m2_d + m2_d
    m2_d = m2_d + m2_d

    Qx = Px * m2_d * C
    Qx = Qx + Qx
    Qx = m2_n - Qx
    Qz = Pz * m2_d * C

    return Qx, Qz


# }}}}}


# Conversions {{{{{


def m2sw(A, Px_m, Pxy_m):
    """Computes isomorphism from Montgomery form to short Weierstrass from

    i.e. computes isomorphism
      phi: E_m: By^2 = x(x^2 + Ax + 1) -> E_w: y^2 = x^3 + ax + b

    and maps
        - x-points in Px_m through phi
        - xy-points in Pxy_m through phi
          These points may be y-twisted, i.e. such that (x, eye * y) lies on E

    Ref: https://eprint.iacr.org/2017/212 Sec 2.4 (Case B = 1)
    """
    # Cost: Curve: 2A/S + 4M + 2I, Per x{,y}-point: 1A/S

    Ad3 = A / 3
    AAd3 = A * Ad3
    a = 1 - AAd3
    b = A * (AAd3 + AAd3 - 3) / 9

    Px_sw = [Px + Ad3 for Px in Px_m]
    Pxy_sw = [[P[0] + Ad3, P[1]] for P in Pxy_m]

    if __debug__:
        p = a.parent().characteristic()
        Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])

        assert all([x**3 + A * x**2 + x in [y**2, -(y**2)] for x, y in Pxy_m])
        assert all([x**3 + a * x + b in [y**2, -(y**2)] for x, y in Pxy_sw])

        sage_E_m = EllipticCurve(Fp2, [0, A, 0, 1, 0])
        sage_E_sw = EllipticCurve(Fp2, [a, b])

        phi = sage_E_m.isomorphism_to(sage_E_sw)

        sage_Px_m = [sage_E_m.lift_x(Px) for Px in Px_m]
        sage_Pxy_m = [sage_E_m.lift_x(Px) for Px, _ in Pxy_m]
        sage_Px_sw = [sage_E_sw.lift_x(Px) for Px in Px_sw]
        sage_Pxy_sw = [sage_E_sw.lift_x(Px) for Px, _ in Pxy_sw]

        assert all([phi(m) in [-sw, sw] for (m, sw) in zip(sage_Px_m, sage_Px_sw, strict=True)])
        assert all([phi(m) in [-sw, sw] for (m, sw) in zip(sage_Pxy_m, sage_Pxy_sw, strict=True)])

    return (a, b), Px_sw, Pxy_sw


def m2sw_proj(E, Pxz_m, Pxyz_m):
    """Computes isomorphism from Montgomery form to short Weierstrass from

    i.e. computes isomorphism
      phi: E_m: By^2 = x(x^2 + Ax + 1) -> E_w: y^2 = x^3 + ax + b

    and maps
        - xz-points in Pxz_m through phi
        - xyz-points in Pxyz_m through phi
          These points may be y-twisted, i.e. such that (x, eye * y) lies on E

    Ref: https://eprint.iacr.org/2017/212 Sec 2.4 (Case B = 1)
    """
    # Cost: Curve: 10A/S + 2M, Per x-point: 1A/S + 2M, Per xy-point: 1A/S + 3M
    A, C = E

    A_pow2 = A**2
    A_pow2_mul2 = A_pow2 + A_pow2
    C_pow2 = C**2

    C_pow2_mul9 = C_pow2 + C_pow2
    C_pow2_mul9 = C_pow2_mul9 + C_pow2_mul9
    C_pow2_mul9 = C_pow2_mul9 + C_pow2_mul9
    C_pow2_mul9 = C_pow2_mul9 + C_pow2

    assert C_pow2_mul9 == 9 * C_pow2

    a = C_pow2_mul9 - A_pow2_mul2 - A_pow2
    b = A * (A_pow2_mul2 - C_pow2_mul9)
    c = 3 * C

    C_mul3 = C + C + C
    Pxz_sw = [[C_mul3 * Pxz[0] + A, C_mul3 * Pxz[1]] for Pxz in Pxz_m]
    Pxyz_sw = [[C_mul3 * Pxyz[0] + A, C_mul3 * Pxyz[1], C_mul3 * Pxyz[2]] for Pxyz in Pxyz_m]

    if __debug__:
        _Px_m = [Pxz[0] / Pxz[1] for Pxz in Pxz_m]
        _Pxy_m = [[Pxyz[0] / Pxyz[2], Pxyz[1] / Pxyz[2]] for Pxyz in Pxyz_m]
        (_a, _b), _Px_sw, _Pxy_sw = m2sw(A / C, _Px_m, _Pxy_m)

        assert _a == a / c**2
        assert _b == b / c**3
        assert all([_Px == Pxz[0] / Pxz[1] for Pxz, _Px in zip(Pxz_sw, _Px_sw, strict=True)])

    return (a, b, c), Pxz_sw, Pxyz_sw


def sw2m(E, Pxy_sw, T8=None):
    # Ref: https://eprint.iacr.org/2017/212 Sec 2.4
    # E: y^2 = x^3 + ax + b -> E: y^2 = x(x^2 + Ax + 1)
    # Concrete implementation is taken from Sage
    #   src/sage/schemes/elliptic_curves/ell_generic.py
    # Where we enfore the twisting factor B = 1
    #
    # When T8 is passed (8-torsion point), it will be used to find a 2-torsion
    # point on the curve, saving a cubic-root finding call

    a, b = E

    if __debug__:
        assert all([x**3 + a * x + b in [-(y**2), y**2] for x, y in Pxy_sw])

    if T8 is not None:
        alpha = double_sw_x(E, double_sw_x(E, T8[0]))
        assert alpha**3 + a * alpha + b == 0
        beta_sq = 3 * alpha**2 + a
        beta = beta_sq.sqrt()
        beta = -beta if not beta.is_square() else beta

    else:
        P = PolynomialRing(a.parent(), "x")
        # alpha^3 + a * alpha + b = 0, i.e. the x-coordinate of a 2-torsion point
        # We can avoid root finding by knowing a point of 2-torsion on E(a, b) already
        # In our applications this is always the case

        # Compatible with sage
        # A more efficient version would be to use a generator next(... if beta.is_square())
        sols = [
            (alpha, beta)
            for alpha in P([b, a, 0, 1]).roots(multiplicities=False)
            for beta in P([3 * alpha**2 + a, 0, -1]).roots(multiplicities=False)
        ]

        # Square s allows us to take B = 1 (i.e. "untwisted")
        alpha, beta = max(sols, key=lambda t: t[1].is_square())

        if __debug__ and a.parent().absolute_degree() == 1:
            # There are 6 montgomery models of the curve
            # 4 of them are defined over fp
            # each in two pairs, A and -A
            # one of each is on the "right" twist
            ctr = 0
            for x in P([b, a, 0, 1]).roots(multiplicities=False):
                rs = P([3 * x**2 + a, 0, -1]).roots(multiplicities=False)
                if rs:
                    assert rs[0].is_square() or rs[1].is_square()
                else:
                    ctr += 1
            assert ctr == 1

    assert alpha**3 + a * alpha + b == 0
    assert 3 * alpha**2 + a == beta**2
    assert beta.is_square()

    # Now
    #   E: y^2 = x^3 + ax + b
    # isomorphic to
    #   E: beta y^2 = x (x^2 + (3 alpha / beta) x + 1)
    # via (x, y) -> ((x - alpha) / beta, y / beta)
    # We want beta = 1, so we need to map through the twisting map
    #   (x, y) -> (x, 1/sqrt(beta) y)

    x_factor = ~beta
    y_factor = x_factor * ~(beta.sqrt())

    A = 3 * alpha * x_factor

    if T8 is None:
        p = a.parent().characteristic()
        Fp = GF(p)
        # i.e. compatible with Sage's Montgomery model choice
        assert EllipticCurve(Fp, [a, b]).montgomery_model() == EllipticCurve(Fp, [0, A, 0, 1, 0])

    Pxy_m = [((x - alpha) * x_factor, y * y_factor) for x, y in Pxy_sw]
    assert all([x * (x**2 + A * x + 1) in [-(y**2), y**2] for x, y in Pxy_m])

    # if __debug__:
    #     p = a.parent().characteristic()
    #     Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])
    #     isos = EllipticCurve(Fp2, [a, b]).isomorphisms(EllipticCurve(Fp2, [0, A, 0, 1, 0]))
    #     iso = next(iso for iso in isos if iso(Pxy_sw[0]).y() == Pxy_m[0][1])
    #     print([iso(sw).xy() == m for m, sw in zip(Pxy_m, Pxy_sw, strict=True)])
    #     assert all([iso(sw).xy() == m for m, sw in zip(Pxy_m, Pxy_sw, strict=True)])

    return A, Pxy_m


def sw2m_proj(E, Pxz_sw, Pxyz_sw, T8xz=None):
    # Ref: https://eprint.iacr.org/2017/212 Sec 2.4
    # E: y^2 = x^3 + ax + b -> E: y^2 = x(x^2 + Ax + 1)
    # Concrete implementation is taken from Sage
    #   src/sage/schemes/elliptic_curves/ell_generic.py
    # Where we enfore the twisting factor B = 1
    #
    # When T8 is passed (8-torsion point), it will be used to find a 2-torsion
    # point on the curve, saving a cubic-root finding call

    if T8xz is None:
        raise NotImplementedError("Must supply 8-torsion point")

    a, b, c = E

    # print([(x / z)**3 + (a / c**2) * (x / z) + (b / c**3) in [-(y**2), y**2] for x, y, z in Pxyz_sw])
    # assert all([(x / z)**3 + (a / c**2) * (x / z) + (b / c**3) in [-(y**2), y**2] for x, y, z in Pxyz_sw])

    c_pow2 = c**2
    c_pow3 = c**3

    alpha_n, alpha_d = double_sw_xz(E, c_pow2, c_pow3, double_sw_xz(E, c_pow2, c_pow3, T8xz))

    alpha = alpha_n / alpha_d
    a_aff = a / c_pow2

    beta_pow2 = 3 * alpha**2 + a_aff
    beta = beta_pow2.sqrt()
    beta = -beta if not beta.is_square() else beta

    assert alpha**3 + (a / c**2) * alpha + (b / c**3) == 0
    assert 3 * alpha**2 + (a / c**2) == beta**2
    assert beta.is_square()

    beta_sqrt = beta.sqrt()
    beta_sqrt = min([beta_sqrt, -beta_sqrt])

    # Now
    #   E: y^2 = x^3 + ax + b
    # isomorphic to
    #   E: beta y^2 = x (x^2 + (3 alpha / beta) x + 1)
    # via (x, y) -> ((x - alpha) / beta, y / beta)
    # We want beta = 1, so we need to map through the twisting map
    #   (x, y) -> (x, 1/sqrt(beta) y)

    # x_factor = ~beta
    # y_factor = x_factor * ~(beta.sqrt())

    A = 3 * alpha
    C = beta

    Pxz_m = [(x - alpha * z, z * beta) for x, y, z in Pxyz_sw]
    Pxyz_m = [((x - alpha * z) * beta, y * beta_sqrt, z * beta_pow2) for x, y, z in Pxyz_sw]
    assert all([(x / z) * ((x / z)**2 + (A / C) * (x / z) + 1) in [-((y / z)**2), (y / z)**2] for x, y, z in Pxyz_m])

    return (A, C), Pxz_m, Pxyz_m


def recover_y(Qx, P, PpQx, E):
    # Ref: https://eprint.iacr.org/2017/212, Algorithm 5
    # Modified to accept a non-normalised projecive point P
    # Cost: 13M + 1S + 5a + 3s
    Qx, Qz = Qx
    Px, Py, Pz = P
    PpQx, PpQz = PpQx
    A = E.a2()

    PxQx = Px * Qx
    PxQz = Px * Qz
    PzQx = Pz * Qx
    PzQz = Pz * Qz
    APzQz_mul2 = 2 * A * PzQz

    # t6 = PpQz * ((PxQx + PzQz) * (PxQz + PzQx + APzQz_mul2) - PzQz * APzQz_mul2) - (PxQz - PzQx)**2 * PpQx
    # t7 = 2 * PpQz * Py * PzQz

    t1 = PxQx + PzQz
    t2 = PxQz + PzQx + APzQz_mul2
    t3 = PzQz * APzQz_mul2
    t4 = (PxQz - PzQx) ** 2
    t5 = PpQz * (t1 * t2 - t3)
    t6 = t5 - t4 * PpQx
    t7 = 2 * PpQz * Py * PzQz

    return Qx * t7, t6, Qz * t7

# }}}}}


if __name__ == "__main__":
    p = 5 * 2**248 - 1
    Fp = GF(p)
    Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])
    (eye,) = Fp2.gens()
    E = random_maximal_curve(p)

    for _ in range(10):
        print(f"# Test: {_ + 1}")
        E = random_maximal_curve(p).montgomery_model()
        P = E.random_element()
        Q = E.random_element()

        r = E.base_ring().random_element()
        Qx, Qy, Qz = [r * Q.x(), r * Q.y(), r]
        r = E.base_ring().random_element()
        Px, Py, Pz = [r * P.x(), r * P.y(), r]
        r = E.base_ring().random_element()
        PpQx, PpQy, PpQz = [r * (P + Q).x(), r * (P + Q).y(), r]

        Q_return = recover_y([Qx, Qz], [Px, Py, Pz], [PpQx, PpQz], E)

        print(Q_return)
        assert Q_return in E

        assert Q_return[0] / Q_return[2] == Qx / Qz
        assert Q_return[1] / Q_return[2] == Qy / Qz

    for _ in range(10):
        print(f"# Test: {_ + 1}")
        P = E.random_element()

        # All functions are self-asserting
        _E = E.a4(), E.a6()
        double_sw_x(_E, P[0])

        # Simulate projective curves/ points
        r = Fp2.random_element()
        s = Fp2.random_element()
        _E = E.a4() * r**2, E.a6() * r**3
        double_sw_xz(_E, r**2, r**3, (P[0] * s, P[2] * s))

    for _ in range(10):
        print(f"# Test: {_ + 1}")
        E_m = random_maximal_curve(p).montgomery_model()

        Px_m = [E_m.random_element() for _ in range(4)]
        Px_m = [P[0] for P in Px_m]
        Pxy_m = [E_m.random_element() for _ in range(4)]
        # Randomly y-twist some points
        Pxy_m = [[P[0], choice([1, eye]) * P[1]] for P in Pxy_m]

        m2sw(E_m.a2(), Px_m, Pxy_m)
