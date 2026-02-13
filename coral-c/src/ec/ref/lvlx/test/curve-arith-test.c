#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

#include "test_extras.h"
#include <ec.h>
#include <rng.h>
#include <bench_test_arguments.h>

/******************************
Test functions
******************************/

int
test__xpoint_eqw_fp2_dbl_xpoint_eqw_fp2_add(const curve_mg_fp2_t *curve, unsigned int Ntest)
{
    unsigned int i;

    xpoint_eqw_fp2_t P, Q, PQ, R1, R2;

    for (i = 0; i < Ntest; i++) {
        ec_random_test(&P, curve);
        ec_random_test(&Q, curve);
        projective_difference_point(&PQ, &P, &Q, curve);

        // 2(P + Q) = 2P + 2Q
        xpoint_eqw_fp2_add(&R1, &P, &Q, &PQ);
        xpoint_eqw_fp2_dbl(&R1, &R1, curve);
        xpoint_eqw_fp2_dbl(&P, &P, curve);
        xpoint_eqw_fp2_dbl(&Q, &Q, curve);
        xpoint_eqw_fp2_dbl(&PQ, &PQ, curve);
        xpoint_eqw_fp2_add(&R2, &P, &Q, &PQ);
        if (!xpoint_eqw_fp2_is_equal(&R1, &R2)) {
            printf("Failed 2(P + Q) = 2P + 2Q\n");
            return 1;
        }

        // (P+Q) + (P-Q) = 2P
        xpoint_eqw_fp2_add(&R1, &P, &Q, &PQ);
        xpoint_eqw_fp2_dbl(&Q, &Q, curve);
        xpoint_eqw_fp2_add(&R1, &R1, &PQ, &Q);
        xpoint_eqw_fp2_dbl(&P, &P, curve);
        xpoint_eqw_fp2_dbl(&PQ, &PQ, curve);
        if (!xpoint_eqw_fp2_is_equal(&R1, &P)) {
            printf("Failed (P+Q) + (P-Q) = 2P\n");
            return 1;
        }
    }

    return 0;
}

int
test__xpoint_eqw_fp2_dbladd(const curve_mg_fp2_t *curve, unsigned int Ntest)
{
    unsigned int i;

    xpoint_eqw_fp2_t P, Q, PQ, R1, R2;

    xpoint_eqw_fp2_t A24;
    curve_mg_fp2_AC_to_A24(&A24, curve);

    for (i = 0; i < Ntest; i++) {
        ec_random_test(&P, curve);
        ec_random_test(&Q, curve);
        projective_difference_point(&PQ, &P, &Q, curve);

        _xpoint_eqw_fp2_dbladd(&R1, &R2, &P, &Q, &PQ, &A24, false);
        xpoint_eqw_fp2_add(&PQ, &P, &Q, &PQ);
        if (!xpoint_eqw_fp2_is_equal(&R2, &PQ)) {
            printf("Failed addition in _xpoint_eqw_fp2_dbladd\n");
            return 1;
        }
        xpoint_eqw_fp2_dbl(&P, &P, curve);
        if (!xpoint_eqw_fp2_is_equal(&R1, &P)) {
            printf("Failed doubling in _xpoint_eqw_fp2_dbladd\n");
            return 1;
        }
    }
    return 0;
}

int
test__xpoint_eqw_fp2_dbl_variants(curve_mg_fp2_t *curve, unsigned int Ntest)
{
    unsigned int i;
    curve_mg_fp2_t E;
    xpoint_eqw_fp2_t P, R1, R2, R3, R4;
    xpoint_eqw_fp2_t A24, A24norm;
    fp2_t z;

    curve_mg_fp2_AC_to_A24(&A24, curve);
    xpoint_eqw_fp2_copy(&A24norm, &A24);
    xpoint_eqw_fp2_normalise(&A24norm);

    // Randomize projective representation
    curve_mg_fp2_copy(&E, curve);
    fp2_random_test(&z);
    fp2_mul(&(E.A24.x), &(A24.x), &z);
    fp2_mul(&(E.A24.z), &(A24.z), &z);
    E.is_A24_computed_and_normalized = false;

    for (i = 0; i < Ntest; i++) {
        ec_random_test(&P, curve);
        _xpoint_eqw_fp2_dbl(&R1, &P, (const xpoint_eqw_fp2_t *)curve);
        _xpoint_eqw_fp2_dbl_A24(&R2, &P, &(E.A24), false);
        _xpoint_eqw_fp2_dbl_A24(&R3, &P, &A24norm, true);
        _xpoint_eqw_fp2_dbl_E0(&R4, &P);
        if (!xpoint_eqw_fp2_is_equal(&R1, &R2)) {
            printf("_xpoint_eqw_fp2_dbl and _xpoint_eqw_fp2_dbl_A24 dont match\n");
            return 1;
        }
        if (!xpoint_eqw_fp2_is_equal(&R1, &R3)) {
            printf("_xpoint_eqw_fp2_dbl and _xpoint_eqw_fp2_dbl_A24 normalized dont match\n");
            return 1;
        }
        if (!xpoint_eqw_fp2_is_equal(&R1, &R4)) {
            printf("_xpoint_eqw_fp2_dbl and _xpoint_eqw_fp2_dbl_E0 dont match\n");
            return 1;
        }
    }
    return 0;
}

int
test_zero_identities(curve_mg_fp2_t *curve, unsigned int Ntest)
{
    unsigned int i;

    xpoint_eqw_fp2_t P, Q, R, ec_zero;

    fp2_set_one(&(P.x));
    fp2_set_zero(&(P.z));

    fp2_set_one(&(ec_zero.x));
    fp2_set_zero(&(ec_zero.z));

    assert(xpoint_eqw_fp2_is_zero(&P));

    for (i = 0; i < Ntest; i++) {
        ec_random_test(&P, curve);

        xpoint_eqw_fp2_add(&R, &ec_zero, &ec_zero, &ec_zero);
        if (!xpoint_eqw_fp2_is_zero(&R)) {
            printf("Failed 0 + 0 = 0\n");
            return 1;
        }

        xpoint_eqw_fp2_dbl(&R, &P, curve);
        xpoint_eqw_fp2_add(&R, &P, &P, &R);
        if (!xpoint_eqw_fp2_is_zero(&R)) {
            printf("Failed P - P = 0\n");
            return 1;
        }

        xpoint_eqw_fp2_dbl(&R, &ec_zero, curve);
        if (!xpoint_eqw_fp2_is_zero(&R)) {
            printf("Failed 2*0 = 0\n");
            return 1;
        }

        xpoint_eqw_fp2_add(&R, &P, &ec_zero, &P);
        if (!xpoint_eqw_fp2_is_equal(&R, &P)) {
            printf("Failed P + 0 = P\n");
            return 1;
        }
        xpoint_eqw_fp2_add(&R, &ec_zero, &P, &P);
        if (!xpoint_eqw_fp2_is_equal(&R, &P)) {
            printf("Failed P + 0 = P\n");
            return 1;
        }

        _xpoint_eqw_fp2_dbladd(&R, &Q, &P, &ec_zero, &P, &curve->A24, false);
        if (!xpoint_eqw_fp2_is_equal(&Q, &P)) {
            printf("Failed P + 0 = P in _xpoint_eqw_fp2_dbladd\n");
            return 1;
        }
        _xpoint_eqw_fp2_dbladd(&R, &Q, &ec_zero, &P, &P, &curve->A24, false);
        if (!xpoint_eqw_fp2_is_equal(&Q, &P)) {
            printf("Failed P + 0 = P in _xpoint_eqw_fp2_dbladd\n");
            return 1;
        }
        if (!xpoint_eqw_fp2_is_zero(&R)) {
            printf("Failed 2*0 = 0 in _xpoint_eqw_fp2_dbladd\n");
            return 1;
        }
    }
    return 0;
}

int
test_jacobian(const curve_mg_fp2_t *curve, unsigned int Ntest)
{
    unsigned int i;

    xpoint_eqw_fp2_t P, Q;
    point_jac_fp2_t R, S, T, U, jac_zero;
    fp2_t t0, t1;

    point_jac_fp2_init(&jac_zero);

    for (i = 0; i < Ntest; i++) {
        ec_random_test(&P, curve);
        xpoint_eqw_fp2_normalise(&P);
        ec_random_test(&Q, curve);
        xpoint_eqw_fp2_normalise(&Q);

        /* Convert to Jacobian coordinates. */
        fp2_copy(&(S.x), &(P.x));
        xpoint_eqw_fp2_recover_y_sqrt(&(S.y), &(S.x), curve);
        fp2_set_one(&(S.z));
        fp2_copy(&(T.x), &(Q.x));
        xpoint_eqw_fp2_recover_y_sqrt(&(T.y), &(T.x), curve);
        fp2_set_one(&(T.z));

        point_jac_fp2_add(&R, &jac_zero, &jac_zero, curve);
        if (!point_jac_fp2_is_equal(&R, &jac_zero)) {
            printf("Failed 0 + 0 = 0 in jac\n");
            return 1;
        }

        point_jac_fp2_dbl(&R, &jac_zero, curve);
        if (!point_jac_fp2_is_equal(&R, &jac_zero)) {
            printf("Failed 2*0 = 0 in jac\n");
            return 1;
        }

        point_jac_fp2_neg(&R, &S);
        point_jac_fp2_add(&R, &S, &R, curve);
        if (!point_jac_fp2_is_equal(&R, &jac_zero)) {
            printf("Failed P - P = 0 in jac\n");
            return 1;
        }

        point_jac_fp2_add(&R, &S, &jac_zero, curve);
        if (!point_jac_fp2_is_equal(&R, &S)) {
            printf("Failed P + 0 = P in jac\n");
            return 1;
        }
        point_jac_fp2_add(&R, &jac_zero, &S, curve);
        if (!point_jac_fp2_is_equal(&R, &S)) {
            printf("Failed P + 0 = P in jac\n");
            return 1;
        }
        point_jac_fp2_add(&R, &S, &jac_zero, curve);
        if (!point_jac_fp2_is_equal(&R, &S)) {
            printf("Failed 0 + P = P in jac\n");
            return 1;
        }

        point_jac_fp2_dbl(&R, &S, curve);
        point_jac_fp2_add(&U, &S, &S, curve);
        if (!point_jac_fp2_is_equal(&R, &U)) {
            printf("Failed P + P = 2*P in jac\n");
            return 1;
        }

        point_jac_fp2_add(&R, &T, &S, curve);
        point_jac_fp2_add(&T, &S, &T, curve);
        if (!point_jac_fp2_is_equal(&R, &T)) {
            printf("Failed P + Q = Q + P in jac\n");
            return 1;
        }
        point_jac_fp2_add(&R, &T, &S, curve);
        point_jac_fp2_add(&U, &S, &T, curve);
        if (!point_jac_fp2_is_equal(&R, &U)) {
            printf("Failed P + Q = Q + P in jac\n");
            return 1;
        }

        // Double R to make it different than (T + S).
        point_jac_fp2_dbl(&R, &R, curve);
        point_jac_fp2_add(&U, &S, &T, curve);
        point_jac_fp2_add(&U, &U, &R, curve);
        point_jac_fp2_add(&R, &R, &T, curve);
        point_jac_fp2_add(&R, &R, &S, curve);
        if (!point_jac_fp2_is_equal(&R, &U)) {
            printf("Failed (P + Q) + R = P + (Q + R) in jac\n");
            return 1;
        }

        jac_to_ws_fp2(&R, &t0, &t1, &jac_zero, curve);
        jac_from_ws_fp2(&R, &R, &t1, curve);
        if (!point_jac_fp2_is_equal(&R, &jac_zero)) {
            printf("Failed converting to Weierstrass\n");
            return 1;
        }

        jac_to_ws_fp2(&R, &t0, &t1, &S, curve);
        jac_from_ws_fp2(&R, &R, &t1, curve);
        if (!point_jac_fp2_is_equal(&S, &R)) {
            printf("Failed converting to Weierstrass\n");
            return 1;
        }
        point_jac_fp2_dbl(&S, &S, curve);
        jac_to_ws_fp2(&R, &t0, &t1, &S, curve);
        jac_from_ws_fp2(&R, &R, &t1, curve);
        if (!point_jac_fp2_is_equal(&S, &R)) {
            printf("Failed converting to Weierstrass\n");
            return 1;
        }

        jac_to_ws_fp2(&R, &t0, &t1, &jac_zero, curve);
        point_jac_fp2_dblw(&R, &t0, &R, &t0);
        jac_from_ws_fp2(&R, &R, &t1, curve);
        if (!point_jac_fp2_is_equal(&R, &jac_zero)) {
            printf("Failed 2*0 = 0 in Weierstrass\n");
            return 1;
        }
        jac_to_ws_fp2(&R, &t0, &t1, &S, curve);
        point_jac_fp2_dblw(&R, &t0, &R, &t0);
        jac_from_ws_fp2(&R, &R, &t1, curve);
        point_jac_fp2_dbl(&S, &S, curve);
        if (!point_jac_fp2_is_equal(&S, &R)) {
            printf("Failed doubling in Weierstrass\n");
            return 1;
        }
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    uint32_t seed[12] = { 0 };
    int iterations = 100 * SQISIGN_TEST_REPS;
    int help = 0;
    int seed_set = 0;
    int res = 0;

    for (int i = 1; i < argc; i++) {
        if (!help && strcmp(argv[i], "--help") == 0) {
            help = 1;
            continue;
        }

        if (!seed_set && !parse_seed(argv[i], seed)) {
            seed_set = 1;
            continue;
        }

        if (sscanf(argv[i], "--iterations=%d", &iterations) == 1) {
            continue;
        }
    }

    if (help || iterations <= 0) {
        printf("Usage: %s [--iterations=<iterations>] [--seed=<seed>]\n", argv[0]);
        printf("Where <iterations> is the number of iterations used for testing; if not "
               "present, uses the default: %d)\n",
               iterations);
        printf("Where <seed> is the random seed to be used; if not present, a random seed is "
               "generated\n");
        return 1;
    }

    if (!seed_set) {
        randombytes_select((unsigned char *)seed, sizeof(seed));
    }

    print_seed(seed);

#if defined(TARGET_BIG_ENDIAN)
    for (int i = 0; i < 12; i++) {
        seed[i] = BSWAP32(seed[i]);
    }
#endif

    randombytes_init((unsigned char *)seed, NULL, 256);

    // Curve A=6
    curve_mg_fp2_t curve;
    curve_mg_fp2_init(&curve);
    fp2_set_small(&(curve.A), 0);
    fp2_set_small(&(curve.C), 1);
    // fp2_random_test(&(curve.C));
    // fp2_mul(&(curve.A), &(curve.A), &(curve.C));
    curve_mg_fp2_normalise_A24(&curve);

    res |= test__xpoint_eqw_fp2_dbl_xpoint_eqw_fp2_add(&curve, iterations);
    res |= test__xpoint_eqw_fp2_dbladd(&curve, iterations);
    res |= test__xpoint_eqw_fp2_dbl_variants(&curve, iterations);
    res |= test_zero_identities(&curve, iterations);
    res |= test_jacobian(&curve, iterations);

    fp2_random_test(&(curve.C));
    fp2_mul(&(curve.A), &(curve.A), &(curve.C));
    curve_mg_fp2_normalise_A24(&curve);

    res |= test__xpoint_eqw_fp2_dbl_xpoint_eqw_fp2_add(&curve, iterations);
    res |= test__xpoint_eqw_fp2_dbladd(&curve, iterations);
    res |= test__xpoint_eqw_fp2_dbl_variants(&curve, iterations);
    res |= test_zero_identities(&curve, iterations);
    res |= test_jacobian(&curve, iterations);

    if (res) {
        printf("Tests failed!\n");
    } else {
        printf("All ec arithmetic tests passed.\n");
    }

    return res;
}
