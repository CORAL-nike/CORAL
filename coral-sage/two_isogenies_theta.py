#!/usr/bin/env python

# A general implementation of 2^*-isogenies between Kummer lines as explained in
#
#   Computing 2-isogenies between Kummer lines
#   Damien Robert and Nicolas Sarkis
#
#   Paper: https://eprint.iacr.org/2024/37
#   Implementation: https://gitlab.inria.fr/nsarkis/poc-scalar-multiplication-kummer-lines
#
# The original implementation is for 2^*-isogenies with kernel P_0 above R_0
# This implementation allows for arbitrary 2^*-isogenies
#
# Two caveats:
#   - This code was originally written for CORAL, and focused on very short
#     chains (of length at most 8), so does not include strategies.
#   - This code does not take advantage of [Cor. 2, 2017/1198], i.e. that when
#     the kernel P_0 lies above R_0, all intermediate kernels will not lie above
#     T_i. As explained in [App. C, 2024/37] this allows one to use the
#     translated 2-isogeny formulae [Th. 4.8, 2024/37], saving 4M per step.
#


from argparse import ArgumentParser
from random import choice

from sage.all import GF, EllipticCurve, classical_modular_polynomial, factor, is_prime, proof

proof.all(False)


def random_maximal_curve(p):
    Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])
    j0 = Fp2(1728)
    j1 = choice(list(set(classical_modular_polynomial(2, j0).roots(Fp2, multiplicities=False)) - set([j0])))

    for _ in range(p.bit_length()):
        mp2 = classical_modular_polynomial(2, j1)
        x = mp2.parent().gens()[0]
        j0, j1 = j1, choice((mp2 // (x - j0)).roots(Fp2, multiplicities=False))

    E = EllipticCurve(j=j1)
    if E.cardinality() != (p + 1) ** 2:
        E = E.quadratic_twist()
        assert E.cardinality() == (p + 1) ** 2
        return E

    return E


def if_double(E, P):
    """(Inversion free) Double point P = (X: Z) on montgomery curve E = (A: C)"""
    # 2024/37 Algorithm 5
    # Cost 10A + 2S + 5M
    A, C = E
    X1, Z1 = P
    u = (X1 + Z1) ** 2
    v = (X1 - Z1) ** 2
    t = u - v
    # Tiny optimisation
    # X2, Z2 = u * v, t * (v + (A / C + 2) * t / 4)
    # X2, Z2 = 4 * u * v * C, t * (4 * v * C + (A + 2 * C) * t)
    uvC = u * v * C
    uv_2 = uvC + uvC
    X2 = uv_2 + uv_2

    vC = v * C
    vC_2 = vC + vC
    Z2 = t * (vC_2 + vC_2 + (A + C + C) * t)

    if __debug__:
        E = EllipticCurve([0, A / C, 0, 1, 0])
        P = E.lift_x(X1 / Z1)
        tP = E.lift_x(X2 / Z2)
        assert 2 * P in [-tP, tP]

    return X2, Z2


def double(E, P):
    """Double point P = (X: Z) on montgomery curve E = (A: 1)"""
    # 2024/37 Algorithm 5
    # Cost 9A + 2S + 3M
    A, C = E

    assert C == 1

    X1, Z1 = P
    u = (X1 + Z1) ** 2
    v = (X1 - Z1) ** 2
    t = u - v
    # Tiny optimisation
    # X2, Z2 = u * v, t * (v + (A + 2) * t / 4)
    # X2, Z2 = 4 * u * v, t * (4 * v + (A + 2) * t)
    uv = u * v
    uv_2 = uv + uv
    X2 = uv_2 + uv_2

    v_2 = v + v
    Z2 = t * (v_2 + v_2 + (A + 2) * t)

    if __debug__:
        E = EllipticCurve([0, A, 0, 1, 0])
        P = E.lift_x(X1 / Z1)
        tP = E.lift_x(X2 / Z2)
        assert 2 * P in [-tP, tP]

    return X2, Z2


def two_isog_R1(E1, R1, R1p, Kp, E1_points):
    # R1p a 4-torsion point lying above R1
    E1_A, E1_C = E1
    a1p, b1p = R1p
    A1, B1 = R1

    a1p_sq, b1p_sq = a1p**2, b1p**2
    A1_sq, B1_sq = A1**2, B1**2
    a1, b1 = a1p + b1p, a1p - b1p

    assert (a1**2 + b1**2) / (a1**2 - b1**2) == A1 / B1

    # Montgomery coefficient given by 2024/37 Eq. 3
    # >  E2_A = 4 * E2_d - 2
    # Giving projetively as E2_A / E2_C
    # Tiny optimisation
    # E2_A = 2 * B1_sq - 4 * A1_sq
    A1_sq_2 = A1_sq + A1_sq
    E2_A = B1_sq + B1_sq - (A1_sq_2 + A1_sq_2)
    E2_C = B1_sq
    E2 = E2_A, E2_C

    R2 = a1p_sq, b1p_sq

    def g(X, Z):
        # Cost 6A + 2S + 6M
        # https://ia.cr/2024/37 Th. 3
        u, v = b1 * (X + Z), a1 * (X - Z)
        fX, fZ = (u + v) ** 2, (u - v) ** 2
        # Translate by R2 = (a1p_sq: b1p_sq) using 2024/37 Eq. 5
        gX, gZ = a1p_sq * fX - b1p_sq * fZ, b1p_sq * fX - a1p_sq * fZ
        return gX, gZ

    K2p = g(*Kp)
    E2_points = [g(*point) for point in E1_points]

    if __debug__:
        assert E2_A / E2_C == 4 * (B1**2 - A1**2) / B1**2 - 2

        our_E1 = EllipticCurve([0, E1_A / E1_C, 0, 1, 0])
        our_E2 = EllipticCurve([0, E2_A / E2_C, 0, 1, 0])

        sage_R1p = our_E1.lift_x(R1p[0] / R1p[1])
        sage_R1 = our_E1.lift_x(R1[0] / R1[1])
        sage_Kp = our_E1.lift_x(Kp[0] / Kp[1])
        sage_K2p = our_E2.lift_x(K2p[0] / K2p[1])

        assert sage_R1p.order() == 4
        assert sage_R1.order() == 2

        sage_phi = our_E1.isogeny(sage_R1)
        sage_E2 = sage_phi.codomain()
        sage_phi = sage_E2.isomorphism_to(our_E2) * sage_phi

        our_E1_points = [our_E1.lift_x(P[0] / P[1]) for P in E1_points]
        our_E2_points = [our_E2.lift_x(P[0] / P[1]) for P in E2_points]

        assert all([sage_phi(P) in [Q, -Q] for P, Q in zip(our_E1_points, our_E2_points, strict=True)])
        assert sage_phi(sage_Kp) in [sage_K2p, -sage_K2p]

    return E2, R2, K2p, E2_points


def two_isog_T1(E1, R1, Kp, E1_points):
    E1_A, E1_C = E1
    A1, B1 = R1
    A1_sq, B1_sq = A1**2, B1**2
    A2, B2 = A1 + B1, A1 - B1

    # Montgomery coefficient given by 2024/37 Eq. 3
    # >  E2_A = 4 * E2_d - 2
    # Giving projetively as E2_A / E2_C
    # Tiny optimisation
    # E2_A = 2 * B1_sq + 2 * A1_sq
    E2_A = B1_sq + B1_sq + A1_sq + A1_sq
    E2_C = B1_sq - A1_sq
    E2 = E2_A, E2_C

    R2 = B2, A2

    def g(X, Z):
        # Cost 4A + 2S + 6M
        # 2024/37 Th. 2
        fX, fZ = B2 * (X + Z) ** 2, A2 * (X - Z) ** 2
        # Translate by S2 = (B2: A2) using 2024/37 Eq. 5
        # (Formula is the same as translation by R2 resp. R1)
        gX, gZ = B2 * fX - A2 * fZ, A2 * fX - B2 * fZ
        return gX, gZ

    K2p = g(*Kp)
    E2_points = [g(*point) for point in E1_points]

    if __debug__:
        assert E2_A / E2_C == 4 * B1**2 / (B1**2 - A1**2) - 2

        our_E1 = EllipticCurve([0, E1_A / E1_C, 0, 1, 0])
        our_E2 = EllipticCurve([0, E2_A / E2_C, 0, 1, 0])

        sage_T1 = our_E1.lift_x(0)
        sage_R1 = our_E1.lift_x(R1[0] / R1[1])
        sage_S2 = our_E2.lift_x(B2 / A2)
        sage_R2 = our_E2.lift_x(B2 / A2)

        assert sage_T1.order() == 2
        assert sage_R1.order() == 2
        assert sage_T1 != sage_R1
        assert sage_S2.order() == 2
        assert sage_R2.order() == 2

        sage_phi = our_E1.isogeny(sage_T1)
        sage_E2 = sage_phi.codomain()
        sage_phi = sage_E2.isomorphism_to(our_E2) * sage_phi

        our_E1_points = [our_E1.lift_x(P[0] / P[1]) for P in E1_points]
        our_E2_points = [our_E2.lift_x(P[0] / P[1]) for P in E2_points]

        assert all([sage_phi(P) in [Q, -Q] for P, Q in zip(our_E1_points, our_E2_points, strict=True)])

    return E2, R2, K2p, E2_points


def two_isog_chain(E, R1, Kp, log_2_K_order, points):
    """Compute isogeny on E = (A: C) with kernel K = 2*Kp and push through `points`"""

    # Kernel of the isogeny to be computed
    K = if_double(E, Kp)

    if __debug__:
        p = E[0].parent().characteristic()
        Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])
        sage_K = EllipticCurve(Fp2, [0, E[0] / E[1], 0, 1, 0]).lift_x(K[0] / K[1])
        sage_Kp = EllipticCurve(Fp2, [0, E[0] / E[1], 0, 1, 0]).lift_x(Kp[0] / Kp[1])
        assert 2 * sage_Kp in [sage_K, -sage_K]
        assert sage_K.order() == 2**log_2_K_order

    for order in range(log_2_K_order, 0, -1):
        # 2-above kernel of next 2-isogeny
        kp = Kp
        for _ in range(order - 1):
            kp = if_double(E, kp)

        # Kernel of next 2-isogeny
        k = if_double(E, kp)

        if __debug__:
            sage_k = EllipticCurve(Fp2, [0, E[0] / E[1], 0, 1, 0]).lift_x(k[0] / k[1])
            sage_kp = EllipticCurve(Fp2, [0, E[0] / E[1], 0, 1, 0]).lift_x(kp[0] / kp[1])
            assert sage_kp.order() == 4
            assert sage_k.order() == 2
            assert 2 * sage_kp in [sage_k, -sage_k]

        if k[0] == 0:
            E, R1, Kp, points = two_isog_T1(E, R1, Kp, points)
        else:
            E, R1, Kp, points = two_isog_R1(E, k, kp, Kp, points)

    return E, points


if __name__ == "__main__":
    parser = ArgumentParser()

    parser.add_argument("-p", "--prime", type=int, default=32, choices=[32, 500, 1000, 2000, 4000])
    parser.add_argument("-r", "--reps", type=int, default=10)
    parser.add_argument("-l", "--length", type=int, default=8)
    args = parser.parse_args()

    def random_point_of_order(E, n):
        while (P := E.random_element() * ((p + 1) // n)).order() != n:
            continue
        assert P.order() == n
        return P

    log_2_K_order = args.length

    # Schema: "human name": (f, c) s.t. p = c * 2**f - 1
    params = {32: (32, 5), 500: (503, 33), 2000: (2026, 51), 4000: (4084, 63)}

    f, c = params[args.prime]
    p = c * 2**f - 1
    assert is_prime(p)
    print(f"p = {factor(p + 1)}")

    for test in range(args.reps):
        print(f"Test {test + 1}/{args.reps}")

        E1 = random_maximal_curve(p).montgomery_model()
        Kp = random_point_of_order(E1, 2 ** (log_2_K_order + 1))
        B1, B2 = E1.torsion_basis(2)
        R = next(point for point in [B1, B2, B1 + B2] if point[0] != 0)

        points = [list(E1.random_element()) for _ in range(4)]
        points = [[point[0], point[2]] for point in points]
        points1 = points[:]

        E1 = (E1.a2(), 1)
        Kp = [Kp[0], Kp[2]]
        R = [R[0], R[2]]

        E2, points2 = two_isog_chain(E1, R, Kp, log_2_K_order, points1)

        our_E1 = EllipticCurve([0, E1[0] / E1[1], 0, 1, 0])
        our_E2 = EllipticCurve([0, E2[0] / E2[1], 0, 1, 0])

        sage_Kp = our_E1.lift_x(Kp[0] / Kp[1])
        sage_phi = our_E1.isogeny(2 * sage_Kp)
        sage_E2 = sage_phi.codomain()
        sage_phi = sage_E2.isomorphism_to(our_E2) * sage_phi

        our_points_1 = [our_E1.lift_x(point[0] / point[1]) for point in points1]
        our_points_2 = [our_E2.lift_x(point[0] / point[1]) for point in points2]

        assert our_points_1[0] in our_E1
        assert our_points_1[0] in sage_phi.domain()

        assert all([sage_phi(our_points_1[i]) in [-our_points_2[i], our_points_2[i]] for i in range(4)])
