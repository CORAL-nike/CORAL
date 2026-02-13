#!/usr/bin/env python

from argparse import ArgumentParser
from itertools import batched

from sage.all import GF, EllipticCurve, factor, is_prime, proof

from batched_inversions import batched_inversion
from ec import double_sw_x, double_sw_xz, m2sw, m2sw_proj
from params import params
from utilities.random_curve import random_maximal_curve

try:
    from sage.rings.finite_rings.ffa_counter import FFA_counter
except (ModuleNotFoundError, ImportError):

    def FFA_counter():
        return 0


proof.all(False)


def isog_eval_sw_x(t, Kx, Px):
    # Cost: 2A/S + 2M + 1I
    PxmKx = Px - Kx
    PxmKx_inv = ~(PxmKx)

    return (Px * PxmKx + t) * PxmKx_inv


def isog_eval_sw_xy(t, Kx, Pxy):
    # Cost: 3A/S + 3M + 2S + 1I

    Px, Py = Pxy
    PxmKx = Px - Kx
    PxmKx_pow2 = PxmKx**2
    PxmKx_inv = ~PxmKx
    PxmKx_inv_pow2 = PxmKx_inv**2

    return (Px * PxmKx + t) * PxmKx_inv, (PxmKx_pow2 - t) * PxmKx_inv_pow2 * Py


def two_isog_chain_sw(E, Kx, log_2_K_order, Pxys):
    a, b = E

    for order in range(log_2_K_order, 0, -1):
        if __debug__:
            p = a.parent().characteristic()
            Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])
            assert EllipticCurve(Fp2, [a, b]).lift_x(Kx).order() == 2**order

        kx = Kx
        for _ in range(order - 1):
            kx = double_sw_x((a, b), kx)

        # i.e. k = (kx : * : *) is a 2-torsion point on E
        assert kx**3 + a * kx + b == 0

        # t = kx**2
        # t = t + t + t + a
        t = 3 * kx**2 + a
        w = kx * t

        Pxys = [isog_eval_sw_xy(t, kx, Pxy) for Pxy in Pxys]

        if order > 1:
            Kx = isog_eval_sw_x(t, kx, Kx)

        t_mul5 = t + t
        t_mul5 = t_mul5 + t_mul5
        t_mul5 = t_mul5 + t
        assert t_mul5 == 5 * t

        a = a - t_mul5

        w_mul7 = w + w
        w_mul7 = w_mul7 + w_mul7
        w_mul7 = w_mul7 + w + w + w
        assert w_mul7 == 7 * w

        b = b - w_mul7

        assert all([x**3 + a * x + b in [y**2, -(y**2)] for x, y in Pxys])

    return (a, b), Pxys


def two_isog_chain_m2sw(A, Kx, log_2_K_order, points):
    [a, b], [Px_sw], points = m2sw(A, [Kx], points)
    [a, b], points = two_isog_chain_sw([a, b], Px_sw, log_2_K_order, points)

    return [a, b], points


def isog_eval_sw_proj_x(T, c_pow2, Kxz, Pxz):
    # Cost 2A/S + 1S + 7M
    Px, Pz = Pxz
    Kx, Kz = Kxz

    d = Px * Kz - Kx * Pz
    Kzdc_pow2 = Kz * d * c_pow2
    Pz2T = Pz**2 * T

    Qx = Px * Kzdc_pow2 + Pz2T
    Qz = Pz * Kzdc_pow2

    return Qx, Qz


def isog_eval_sw_proj_xy(T, c_pow2, Kxz, P):
    # Cost 3A/S + 2S + 11M
    Px, Py, Pz = P
    Kx, Kz = Kxz

    d = Px * Kz - Kx * Pz
    d2C2 = d**2 * c_pow2
    Pz2T = Pz**2 * T

    Qx = Kz * Px * d2C2 + Pz2T * d
    Qy = (d2C2 - Pz2T) * Py * Kz
    Qz = Pz * Kz * d2C2

    return Qx, Qy, Qz


def two_isog_chain_sw_proj(E, Kxz, log_2_K_order, points):
    A, B, C = E

    for order in range(log_2_K_order, 0, -1):
        if __debug__:
            p = A.parent().characteristic()
            Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])
            assert EllipticCurve(Fp2, [A / C**2, B / C**3]).lift_x(Kxz[0] / Kxz[1]).order() == 2**order

        C_pow2 = C**2
        C_pow3 = C * C_pow2
        E = A, B, C

        kxz = Kxz
        for _ in range(order - 1):
            kxz = double_sw_xz(E, C_pow2, C_pow3, kxz)

        kx, kz = kxz

        # i.e. k = (kx : * : kz) is a 2-torsion point on E
        assert (kx / kz) ** 3 + (A / C**2) * (kx / kz) + (B / C**3) == 0

        # Recall t = 3 * (Kx / Kz)**2 + a, define T := C**2 * Kz**2 * t
        T = C_pow2 * kx**2
        T = T + T + T
        T = T + A * kz**2
        # Recall w = (Kx / Kz) * t, define W := C**2 * Kz**3 * w
        W = kx * T

        points = [isog_eval_sw_proj_xy(T, C_pow2, kxz, point) for point in points]

        if order > 1:
            Kxz = isog_eval_sw_proj_x(T, C_pow2, kxz, Kxz)

        Kz2 = kz**2
        Kz3 = Kz2 * kz

        T_mul5 = T + T
        T_mul5 = T_mul5 + T_mul5 + T
        assert T_mul5 == 5 * T

        A = A * Kz2 - T_mul5

        WC = W * C
        WC_mul7 = WC + WC
        WC_mul7 = WC_mul7 + WC_mul7
        WC_mul7 = WC_mul7 + WC + WC + WC
        assert WC_mul7 == 7 * WC

        B = B * Kz3 - WC_mul7
        C = C * kz

        assert all([
            (x / z) ** 3 + (A / C**2) * (x / z) + (B / C**3) in [(y / z) ** 2, -((y / z) ** 2)] for x, y, z in points
        ])

    return (A, B, C), points


def two_isog_chain_proj_m2sw(E, Pxz, log_2_P_order, xyz_points_m):
    A, C = E
    [a, b, c], [Pxz_sw], xyz_points_sw = m2sw_proj([A, C], [Pxz], xyz_points_m)
    [a, b, c], xyz_points_sw = two_isog_chain_sw_proj([a, b, c], Pxz_sw, log_2_P_order, xyz_points_sw)

    C_pow2 = c**2
    C_pow3 = C_pow2 * c

    pairs = [[a, C_pow2], [b, C_pow3]]
    for X, Y, Z in xyz_points_sw:
        pairs += [[X, Z], [Y, Z]]

    a, b, *xy_points_sw = batched_inversion(pairs)

    # Unflatten the list [x1, y1, x2, y2] -> [[x1, y1], [x2, y2]]
    xy_points_sw = list(batched(xy_points_sw, 2, strict=True))

    return [a, b], xy_points_sw


if __name__ == "__main__":

    def random_point_of_order(E, n):
        while (P := E.random_element() * ((p + 1) // n)).order() != n:
            continue
        assert P.order() == n
        return P

    parser = ArgumentParser()

    parser.add_argument("-p", "--prime", type=int, default=500, choices=[32, 500, 1000, 2000, 4000])
    parser.add_argument("-r", "--reps", type=int, default=10)
    parser.add_argument("-l", "--length", type=int, default=8)
    parser.add_argument("-t", "--time-arithmetic", type=bool, default=False)
    args = parser.parse_args()

    log_2_P_order = args.length

    f, c = params[args.prime]
    p = c * 2**f - 1
    assert is_prime(p)
    print(f"p = {factor(p + 1)} - 1")

    Fp = GF(p)
    Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])
    (eye,) = Fp2.gens()

    n_points = 4

    for test in range(args.reps):
        print(f"Test {test + 1}/{args.reps}")

        # Affine version
        E1 = random_maximal_curve(p).montgomery_model()
        P = random_point_of_order(E1, 2**log_2_P_order)

        sage_points_1 = [E1.random_element() for _ in range(n_points)]
        xy_points_1 = [[P[0], P[1]] for P in sage_points_1]

        ff0 = FFA_counter()
        [a, b], xy_points_2 = two_isog_chain_m2sw(E1.a2(), P.x(), log_2_P_order, xy_points_1)
        print("Affine")
        print(FFA_counter() - ff0)

        E2 = EllipticCurve([a, b])
        sage_points_2 = [E2((x, y)) for x, y in xy_points_2]

        sage_phi = E1.isogeny(P)
        # Isogeny is only defined up to isomorphism
        isos = sage_phi.codomain().isomorphisms(E2)
        iso = next(iso for iso in isos if iso(sage_phi(sage_points_1[0])) == sage_points_2[0])
        assert all([iso(sage_phi(p1)) == p2 for p1, p2 in zip(sage_points_1, sage_points_2, strict=True)])

        # Projective version
        E1 = random_maximal_curve(p).montgomery_model()
        P = random_point_of_order(E1, 2**log_2_P_order)

        sage_points_1 = [E1.random_element() for _ in range(n_points)]
        xyz_points_1 = [[P[0], P[1], 1] for P in sage_points_1]

        ff0 = FFA_counter()
        r = Fp2.random_element()
        [a, b], xy_points_2 = two_isog_chain_proj_m2sw([E1.a2() * r, r], [P.x(), 1], log_2_P_order, xyz_points_1)
        print("Projective")
        print(FFA_counter() - ff0)

        E2 = EllipticCurve([a, b])
        sage_points_2 = [E2((x, y)) for x, y in xy_points_2]

        sage_phi = E1.isogeny(P)
        # Isogeny is only defined up to isomorphism
        isos = sage_phi.codomain().isomorphisms(E2)
        iso = next(iso for iso in isos if iso(sage_phi(sage_points_1[0])) == sage_points_2[0])
        assert all([iso(sage_phi(p1)) == p2 for p1, p2 in zip(sage_points_1, sage_points_2, strict=True)])
