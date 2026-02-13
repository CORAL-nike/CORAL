#!/usr/bin/env python3

from sage.all import GF, EllipticCurve

from ec import m2sw_proj, sw2m_proj
from libs.pegasis.xonly import isMontgomery
from libs.two_isogenies.theta_structures.couple_point import CouplePoint
from two_isogenies_velu import two_isog_chain_sw_proj


def prepare_hd_kernel(E, P, Q, ideal, strategy, stats=None, C_friendly=True, montgomery=False):
    # Evaluating the quadratic endomorphism, plus first chain endomorphism
    # The first step has been manually computed as in Qlapoti, Page 24

    # If this is not true, then {P,Q}{1,2}_mult are incorrectly computed
    assert P.order() == Q.order() == 2 ** (ideal.e + 2)

    P1 = ideal.P1_mult * P
    P2 = ideal.P2_mult * P

    Q1 = ideal.Q1_mult * Q
    Q2 = ideal.Q2_mult * Q

    if __debug__:
        # Set order to make asserts a bit faster
        P1.set_order(multiple=2 ** (ideal.e + 2))
        P2.set_order(multiple=2 ** (ideal.e + 2))
        Q1.set_order(multiple=2 ** (ideal.e + 2))
        Q2.set_order(multiple=2 ** (ideal.e + 2))

        assert P1.order() == 2 ** (ideal.e + 2 - 1) or Q1.order() == 2 ** (ideal.e + 2 - 1)
        assert P2.order() == 2 ** (ideal.e + 2 - 1) or Q2.order() == 2 ** (ideal.e + 2 - 1)

    if stats is not None:
        stats.update("Evaluating endomorphism")
        stats.update("# Diagonal steps", nondiff=ideal.v)

    # Notation matches Qlapoti, Page 25
    # We must subtract 2, because in Qlapoti
    #   v = v_2(\nrd(\theta^{+}/2))
    #     = v_2(\nrd(\theta^{+})/4)
    #     = v_2(\nrd(\theta^{+})) - 2
    #
    # Recall: In our case \theta^{+} = q + x + y\pi
    # Note that out of each pair (with D = 2**((ideal.e + 2) - (ideal.v + 1))
    #   (D * P1, D * P2)
    #   (D * Q1, D * Q2)
    # at least one point must have full order 2**v
    # Hence if D * P1 does not have full order, then D * P2 must
    # When computed as above, we see that when s_v2 < t_v2, P2 has a larger order than Q2
    # This allows us to use the following selection criterion for deciding

    Fp = GF(E.base_field().characteristic())
    Fp2 = GF((E.base_field().characteristic(), 2), name="i", modulus=[1, 0, 1])

    # Optimisation:
    #   We need D * {P,Q}{1,2} to compute the kernel of the diagonal isogenies (phi_1, phi_2)
    #   ...but then the kernel of the gluing isogeny is given by
    #     D_ * phi_{1,2}({P,Q}{1,2}) = phi_{1,2}(D_ * {P,Q}{1,2})
    #   Since we need to compute D_ * {P,Q}{1,2} anyway when computing D * {P,Q}{1,2},
    #   we simply save this result

    # We can go further, and cache all doublings that the strategy would initially compute
    # (which are then pushed through the gluing)
    # This essentially means that the initial cost of doublings is amortised as it usually is and not wasted
    # We cache these results here
    hd_chain_length = ideal.e - (ideal.v + 1)
    doubles_to_cache = []
    total = 0
    idx = 0
    while total != hd_chain_length - 1:
        total += strategy[idx]
        doubles_to_cache += [strategy[idx]]
        idx += 1

    assert doubles_to_cache[0] != 0

    cached_doubles = []

    P1_ = P1
    P2_ = P2
    Q1_ = Q1
    Q2_ = Q2

    for double_by in doubles_to_cache:
        P1_ = 2**double_by * P1_
        P2_ = 2**double_by * P2_
        Q1_ = 2**double_by * Q1_
        Q2_ = 2**double_by * Q2_
        cached_doubles += [(P1_, P2_, Q1_, Q2_)]

    assert cached_doubles[-1] == (P1_, P2_, Q1_, Q2_)

    # Compute the kernel of the diagaonal isogenies
    if C_friendly:
        if ideal.t_v2 < ideal.s_v2:
            # @optimisation: Should be multiplied x-only, but is only multiplication by 8
            K1x = (8 * P1_).x()
            K2x = -(8 * Q2_).x()
        else:
            # @optimisation: Should be multiplied x-only, but is only multiplication by 8
            K1x = -(8 * Q1_).x()
            K2x = (8 * P2_).x()

        assert E.change_ring(Fp2).lift_x(K1x).order() == 2**ideal.v
        assert E.change_ring(Fp2).lift_x(K2x).order() == 2**ideal.v

    else:
        E = E.change_ring(Fp2)
        eye = Fp2.gens()[0]
        if ideal.t_v2 < ideal.s_v2:
            K1 = E(8 * P1_)
            K2 = 8 * Q2_
            K2 = E(-K2.x(), eye * K2.y())
        else:
            K1 = 8 * Q1_
            K1 = E(-K1.x(), eye * K1.y())
            K2 = E(8 * P2_)

        def twist(P):
            return (-P.x(), eye * P.y())

        P1, P2 = E(P1), E(P2)
        Q1, Q2 = E(twist(Q1)), E(twist(Q2))

        cached_doubles = [[E(p1), E(p2), E(twist(q1)), E(twist(q2))] for p1, p2, q1, q2 in cached_doubles]

        assert K1.order() == 2**ideal.v
        assert K2.order() == 2**ideal.v

        K1.set_order(2**ideal.v)
        K2.set_order(2**ideal.v)

    if stats is not None:
        stats.update("Finalising diagonal kernel")

    if C_friendly:
        assert isMontgomery(E)

        def x_twist(P):
            return [-P[0], P[1], 1]

        def y_twist(P):
            # Multiplication by eye (Don't actually need a multiplication for this)
            if len(list(P)) == 2:
                return [P[0], Fp2([0, P[1]])]
            else:
                return [P[0], Fp2([0, P[1]]), P[2]]

        phi_1_pts = [list(P1), list(x_twist(Q1.xy()))]
        for p1, _, q1, _ in cached_doubles:
            phi_1_pts += [list(p1), list(x_twist(q1.xy()))]

        K1_m = [K1x, Fp(1)]
        E_m = [E.a2(), 1]
        E_sw, [K1_sw], phi_1_pts_sw = m2sw_proj(E_m, [K1_m], phi_1_pts)
        E1_sw, phi_1_pts_mapped_sw = two_isog_chain_sw_proj(E_sw, K1_sw, ideal.v, phi_1_pts_sw)
        T8xz = [phi_1_pts_mapped_sw[-2][0], phi_1_pts_mapped_sw[-2][2]]
        E1_m, _, phi_1_pts_mapped_m = sw2m_proj(E1_sw, [], phi_1_pts_mapped_sw, T8xz=T8xz)

        # Slightly ugly / non-pythonic for better similarity to C-implementation
        # We need to y-twist the q-points
        for i in range(1, len(phi_1_pts_mapped_m), 2):
            phi_1_pts_mapped_m[i] = y_twist(phi_1_pts_mapped_m[i])

        A1 = E1_m[0] / E1_m[1]

        phi_2_pts = [list(P2), list(x_twist(Q2.xy()))]
        for _, p2, _, q2 in cached_doubles:
            phi_2_pts += [list(p2), list(x_twist(q2.xy()))]

        K2_m = [K2x, Fp(1)]
        E_m = [E.a2(), 1]
        E_sw, [K2_sw], phi_2_pts_sw = m2sw_proj(E_m, [K2_m], phi_2_pts)
        E2_sw, phi_2_pts_mapped_sw = two_isog_chain_sw_proj(E_sw, K2_sw, ideal.v, phi_2_pts_sw)
        T8xz = [phi_2_pts_mapped_sw[-2][0], phi_2_pts_mapped_sw[-2][2]]
        E2_m, _, phi_2_pts_mapped_m = sw2m_proj(E2_sw, [], phi_2_pts_mapped_sw, T8xz=T8xz)

        # Slightly ugly / non-pythonic for better similarity to C-implementation
        # We need to y-twist the q-points
        for i in range(1, len(phi_2_pts_mapped_m), 2):
            phi_2_pts_mapped_m[i] = y_twist(phi_2_pts_mapped_m[i])

        A2 = E2_m[0] / E2_m[1]

        # Collecting statistics "early" to exlude "unnecessary" operation counts below
        # (Not for "cheating" but so that we can be sure that the counts are related to our algorithms, and not Sage
        # weirdness)
        if stats is not None:
            stats.update("Computing diagonal isogenies")

        # This costs a bunch of Fp2 operations (Sage being Sage)
        E1 = EllipticCurve(Fp2, [0, A1, 0, 1, 0])
        E2 = EllipticCurve(Fp2, [0, A2, 0, 1, 0])

        phi_1_pts_mapped_m = [E1(pt) for pt in phi_1_pts_mapped_m]
        phi_2_pts_mapped_m = [E2(pt) for pt in phi_2_pts_mapped_m]

        # This doesn't cost any Fp2 operations
        kernel = (
            CouplePoint(phi_1_pts_mapped_m[0], phi_2_pts_mapped_m[0]),
            CouplePoint(phi_1_pts_mapped_m[1], phi_2_pts_mapped_m[1]),
        )

        assert len(phi_1_pts_mapped_m) == len(phi_2_pts_mapped_m)

        # Slightly ugly / non-pythonic for better similarity to C-implementation
        cached_doubles = []
        for i in range(2, len(phi_1_pts_mapped_m), 2):
            cached_doubles += [
                (
                    CouplePoint(phi_1_pts_mapped_m[i], phi_2_pts_mapped_m[i]),
                    CouplePoint(phi_1_pts_mapped_m[i + 1], phi_2_pts_mapped_m[i + 1]),
                )
            ]

    else:
        # Comment in/out "montgomery" model version to compare to "C_friendly" implementation
        # For some reason, the HD library computes the correct thing, even when the codomain of phi_{1,2} are not in
        # Montgomery form
        # (Converting from weierstrass to montgomery is very expensive)
        if montgomery:
            phi_1 = E.isogeny(K1, algorithm="factored", model="montgomery")
            phi_2 = E.isogeny(K2, algorithm="factored", model="montgomery")
        else:
            phi_1 = E.isogeny(K1, algorithm="factored")
            phi_2 = E.isogeny(K2, algorithm="factored")

        kernel = CouplePoint(phi_1(P1), phi_2(P2)), CouplePoint(phi_1(Q1), phi_2(Q2))
        cached_doubles = [
            (CouplePoint(phi_1(p1), phi_2(p2)), CouplePoint(phi_1(q1), phi_2(q2)))
            for (p1, p2, q1, q2) in cached_doubles
        ]

        if stats is not None:
            stats.update("Computing diagonal isogenies")

    return kernel, cached_doubles, stats
