#include "nike.h"
#include "quaternion_data.h"
#include "quaternion_constants.h"

int
nike_normeq(ibz_t *x, ibz_t *y, ibz_t *a, const ibz_t *p, const ibz_t *two_e)
{
    int res = 0;
    ibz_t diff, bound;
    ibz_cornacchia_trial_division_consts_t consts;
    ibz_cornacchia_trial_division_output_t factorization;
    uint16_t bhe[QUAT_bad_primes_length];
    uint16_t ghe[QUAT_good_primes_length];
    char gr[QUAT_good_primes_length];
    ibz_init(&diff);
    ibz_init(&bound);
    consts.good_primes = QUAT_good_primes_list;
    consts.bad_primes = QUAT_bad_primes_list;
    consts.good_primes_length = QUAT_good_primes_length;
    consts.bad_primes_length = QUAT_bad_primes_length;
    consts.primality_test_iterations = QUAT_primality_num_iter;
    factorization.bad_half_exponents = bhe;
    factorization.good_half_exponents = ghe;
    factorization.good_remainders = gr;
    ibz_div(&bound, &diff, two_e, p);
    ibz_sqrt_floor(&bound, &bound);
    ibz_div(&bound, &diff, &bound, &ibz_const_two);
    while (!res) {
        ibz_rand_interval(y, &ibz_const_zero, &bound);
        ibz_add(&diff, y, y);
        ibz_add(&diff, &diff, &ibz_const_one);
        ibz_mul(&diff, y, y);
        ibz_mul(&diff, &diff, p);
        ibz_sub(&diff, two_e, &diff);
        res = ibz_cornacchia_extended_trials(a, x, &factorization, &diff, &consts);
    }
    ibz_finalize(&diff);
    ibz_finalize(&bound);
    return (res);
}


void
coral_secret_key_postinit(coral_secret_key_t *sk)
{
    memset(sk->P1_mult, 0, NWORDS_FIELD * sizeof(*sk->P1_mult));
    memset(sk->P2_mult, 0, NWORDS_FIELD * sizeof(*sk->P1_mult));
    memset(sk->Q1_mult, 0, NWORDS_FIELD * sizeof(*sk->P1_mult));
    memset(sk->Q2_mult, 0, NWORDS_FIELD * sizeof(*sk->P1_mult));

    ibz_t tmp0, tmp1, tmp2, mult;
    ibz_init(&tmp0);
    ibz_init(&tmp1);
    ibz_init(&tmp2);
    ibz_init(&mult);

    // tmp1 = (q + x)**2
    ibz_add(&tmp1, &sk->q, &sk->x);
    ibz_mul(&tmp1, &tmp1, &tmp1);
    // tmp2 = p * y**2
    ibz_mul(&tmp2, &sk->y, &sk->y);
    ibz_mul(&tmp2, &tmp2, &sk->p);
    // tmp1 = (q + x)**2 + p * y**2
    ibz_add(&tmp1, &tmp1, &tmp2);
    // v = valuation(tmp1, 2)
    sk->v = ibz_two_adic(&tmp1);
    sk->v -= 2;

    // Later will compute
    //   (q + x + y) * P1
    //   (q - x - y) * P2
    // Since we only care about this couple up to scalars,
    // and P1.order() == P2.order() == 2*(e + 2) we actually compute
    //   g = gcd(q + x - y, 2**(e + 2))
    //   inverse_mod((q - x - y) / g, 2**(e + 2)) * (q + x + y) * P1
    //   g * P2
    // This is more efficient, because g is a power of 2, so only requires doubling
    // Also, in practice, we see that log_2(g) is small

    int r;
    // tmp0 = 2**(e + 1)
    ibz_pow(&tmp0, &ibz_const_two, sk->e + 1);
    // tmp1 = 2**(e + 2)
    ibz_pow(&tmp1, &ibz_const_two, sk->e + 2);

    // P1
    // mult = q - x - y
    ibz_sub(&mult, &sk->q, &sk->x);
    ibz_sub(&mult, &mult, &sk->y);
    // s_v2 = valuation(mult, 2)
    sk->s_v2 = ibz_two_adic(&mult);
    // mult = mult / 2**s_v2 (i.e. "odd part")
    ibz_div_2exp(&mult, &mult, sk->s_v2);
    // mult = inverse_mod(mult, 2**(e + 2)
    r = ibz_invmod(&mult, &mult, &tmp1);
    assert (r);
    // mult = mult * (q + x + y)
    ibz_add(&tmp2, &sk->q, &sk->x);
    ibz_add(&tmp2, &tmp2, &sk->y);
    ibz_mul(&mult, &mult, &tmp2);
    ibz_mod(&mult, &mult, &tmp1);

    // Reduce to [-2^{e + 1} + 1, 2^{e+1}]
    if (ibz_cmp(&mult, &tmp0) > 0) {
        sk->P1_neg = 1;
        ibz_sub(&mult, &mult, &tmp1);
    } else {
        sk->P1_neg = 0;
    }
    ibz_to_digits(sk->P1_mult, &mult);
    sk->P1_mult_bits = ibz_size_in_base(&mult, 2);

    // P2
    // P2_mult = 2**s_v2
    ibz_pow(&mult, &ibz_const_two, sk->s_v2);
    ibz_mod(&mult, &mult, &tmp1);
    // This is (almost certainly) less than 2^{e+1}, but we reduce anyway to
    // prevent future footguns
    // Reduce to [-2^{e + 1} + 1, 2^{e+1}]
    if (ibz_cmp(&mult, &tmp0) > 0) {
        sk->P2_neg = 1;
        ibz_sub(&mult, &mult, &tmp1);
    } else {
        sk->P2_neg = 0;
    }
    ibz_to_digits(sk->P2_mult, &mult);
    sk->P2_mult_bits = ibz_size_in_base(&mult, 2);

    // ...need to do the same for Q1, Q2
    // ...here the scalars are q + x - y, q - x + y

    // Q1
    // mult = q - x + y
    ibz_sub(&mult, &sk->q, &sk->x);
    ibz_add(&mult, &mult, &sk->y);
    // t_v2 = valuation(mult, 2)
    sk->t_v2 = ibz_two_adic(&mult);
    // mult = mult / 2**t_v2 (i.e. "odd part")
    ibz_div_2exp(&mult, &mult, sk->t_v2);
    // mult = inverse_mod(mult, 2**(e + 2)
    r = ibz_invmod(&mult, &mult, &tmp1);
    assert (r);
    // mult = mult * (q + x - y)
    ibz_add(&tmp2, &sk->q, &sk->x);
    ibz_sub(&tmp2, &tmp2, &sk->y);
    ibz_mul(&mult, &mult, &tmp2);
    ibz_mod(&mult, &mult, &tmp1);

    // Reduce to [-2^{e + 1} + 1, 2^{e+1}]
    if (ibz_cmp(&mult, &tmp0) > 0) {
        sk->Q1_neg = 1;
        ibz_sub(&mult, &mult, &tmp1);
    } else {
        sk->Q1_neg = 0;
    }
    ibz_to_digits(sk->Q1_mult, &mult);
    sk->Q1_mult_bits = ibz_size_in_base(&mult, 2);

    // Q2_mult = 2**t_v2
    ibz_pow(&mult, &ibz_const_two, sk->t_v2);
    ibz_mod(&mult, &mult, &tmp1);

    // This is (almost certainly) less than 2^{e+1}, but we reduce anyway to
    // prevent future footguns
    // Reduce to [-2^{e + 1} + 1, 2^{e+1}]
    if (ibz_cmp(&mult, &tmp0) > 0) {
        sk->Q2_neg = 1;
        ibz_sub(&mult, &mult, &tmp1);
    } else {
        sk->Q2_neg = 0;
    }
    ibz_to_digits(sk->Q2_mult, &mult);
    sk->Q2_mult_bits = ibz_size_in_base(&mult, 2);

    sk->postinit = 1;

    ibz_finalize(&tmp0);
    ibz_finalize(&tmp1);
    ibz_finalize(&tmp2);
    ibz_finalize(&mult);

    // Suppress `-Werror=unused-but-set-variable` in release builds
    (void) r;
}





// Inplace x-twist
void
x_twist_i(point_eqw_fp_t *P) {
    fp_neg(&P->x, &P->x);
}

void
x_xtwist_i(xpoint_eqw_fp_t *P) {
    fp_neg(&P->x, &P->x);
}

// Inplace y-twist
void
y_twist_i(point_jac_fp2_t *P) {
    // i.e. multiplying P.y by eye
    // eye * (a + eye * b) = (-b + eye * a)
    fp_t im;
    fp_copy(&im, &P->y.im);
    fp_copy(&P->y.im, &P->y.re);
    fp_copy(&P->y.re, &im);
    fp_neg(&P->y.re, &P->y.re);
}

void
coral_prepare_hd_kernel(
        theta_couple_jac_point_fp2_t *C1_jac_dbls,
        theta_couple_jac_point_fp2_t *C2_jac_dbls,
        theta_couple_curve_fp_t *E12,
        uint16_t *strategy,
        uint16_t strategy_length,
        const coral_secret_key_t *sk,
        point_eqw_fp_t *P,
        curve_mg_fp_t *E,
        point_eqw_fp_t *Q,
        curve_mg_fp_t *Et
) {
    // clock_t start;

    assert(fp_is_one(&P->z));
    assert(fp_is_one(&Q->z));

    point_eqw_fp_t P1, P2, Q1, Q2;

        // clock_t start_kernel_preparation = clock();
        // start = clock();
    point_eqw_fp_mul(&P1, sk->P1_mult, sk->P1_mult_bits, P, E);
    point_eqw_fp_mul(&P2, sk->P2_mult, sk->P2_mult_bits, P, E);
    point_eqw_fp_mul(&Q1, sk->Q1_mult, sk->Q1_mult_bits, Q, Et);
    point_eqw_fp_mul(&Q2, sk->Q2_mult, sk->Q2_mult_bits, Q, Et);

    if (sk->P1_neg) point_eqw_fp_neg(&P1, &P1);
    if (sk->P2_neg) point_eqw_fp_neg(&P2, &P2);
    if (sk->Q1_neg) point_eqw_fp_neg(&Q1, &Q1);
    if (sk->Q2_neg) point_eqw_fp_neg(&Q2, &Q2);
        // printf(":: [Kernel Preparation] Multiplication took %.3Lf ms\n", (long double)(clock() - start) / CLOCKS_PER_SEC * 1000);

    #ifndef NDEBUG
        assert(point_eqw_fp_on_mg_curve(&P1, E));
        assert(point_eqw_fp_on_mg_curve(&P2, E));
        assert(point_eqw_fp_on_mg_curve(&Q1, Et));
        assert(point_eqw_fp_on_mg_curve(&Q2, Et));
    #endif

        // start = clock();
    point_eqw_fp_t cached_doubles[MAX_STRATEGY_LENGTH][4];
    point_eqw_fp_copy(&cached_doubles[0][0], &P1);
    point_eqw_fp_copy(&cached_doubles[0][1], &P2);
    point_eqw_fp_copy(&cached_doubles[0][2], &Q1);
    point_eqw_fp_copy(&cached_doubles[0][3], &Q2);

    curve_mg_fp_normalise_A24(E);

    for (uint32_t i = 1; i < strategy_length; i++) {
        // The modified strategy, to account for expensive gluing (does not
        // account for fp vs fp)
        const unsigned num_dbls = strategy[i - 1] >= 16 ? strategy[i - 1] / 2 : strategy[i - 1] - 1;
        point_eqw_fp_dbl_iter(&cached_doubles[i][0], num_dbls, &cached_doubles[i - 1][0], E);
        point_eqw_fp_dbl_iter(&cached_doubles[i][1], num_dbls, &cached_doubles[i - 1][1], E);
        point_eqw_fp_dbl_iter(&cached_doubles[i][2], num_dbls, &cached_doubles[i - 1][2], Et);
        point_eqw_fp_dbl_iter(&cached_doubles[i][3], num_dbls, &cached_doubles[i - 1][3], Et);
    }
        // printf(":: [Kernel Preparation] Doubling took %.3Lf ms\n", (long double)(clock() - start) / CLOCKS_PER_SEC * 1000);
        // printf("=> [Kernel Preparation] Total took %.3Lf ms\n", (long double)(clock() - start_kernel_preparation) / CLOCKS_PER_SEC * 1000);

        // clock_t start_diagonal_isogenies = clock();
    point_eqw_fp_t P1_, P2_, Q1_, Q2_;
    point_eqw_fp_copy(&P1_, &cached_doubles[strategy_length - 1][0]);
    point_eqw_fp_copy(&P2_, &cached_doubles[strategy_length - 1][1]);
    point_eqw_fp_copy(&Q1_, &cached_doubles[strategy_length - 1][2]);
    point_eqw_fp_copy(&Q2_, &cached_doubles[strategy_length - 1][3]);

    xpoint_eqw_fp_t K1, K2;
    xpoint_eqw_fp_init(&K1);
    xpoint_eqw_fp_init(&K2);

    point_eqw_fp_t tmp;
    point_eqw_fp_init(&tmp);

        // start = clock();
    if (sk->t_v2 < sk->s_v2) {
        // printf("First case\n");
        // Optimise: Could/Should be done x-only
        point_eqw_fp_dbl_iter(&tmp, 3, &P1_, E);
        point_eqw_fp_to_xpoint(&K1, &tmp);

        point_eqw_fp_dbl_iter(&tmp, 3, &Q2_, Et);
        point_eqw_fp_to_xpoint(&K2, &tmp);
        x_xtwist_i(&K2);
    } else {
        // printf("Second case\n");
        // Optimise: Could/Should be done x-only
        point_eqw_fp_dbl_iter(&tmp, 3, &Q1_, Et);
        point_eqw_fp_to_xpoint(&K1, &tmp);
        x_xtwist_i(&K1);

        point_eqw_fp_dbl_iter(&tmp, 3, &P2_, E);
        point_eqw_fp_to_xpoint(&K2, &tmp);
    }
        // printf(":: [Diagonal isogenies] Kernel preparation took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

    xpoint_eqw_fp_t _, __;

    // 1
        // start = clock();
    point_eqw_fp_t phi_1_pts_eqw[2 * MAX_STRATEGY_LENGTH];
    for (int i = 0; i < strategy_length; i++) {
        // i.e. "2**n_i * P1"
        point_eqw_fp_copy(phi_1_pts_eqw + 2 * i, &cached_doubles[i][0]);
        // i.e. "2**n_i * P2" [sic]
        point_eqw_fp_copy(phi_1_pts_eqw + 2 * i + 1, &cached_doubles[i][2]);
        // The Q-points lie on the twist, we only map the x-coordinate
        // The y-coordinate is twisted after the diagonal isogenies have been
        // computed
        x_twist_i(phi_1_pts_eqw + 2 * i + 1);
    }
        // printf(":: [Diagonal isogenies] (1) Copying points took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

        // start = clock();
    curve_sw_fp_t E_sw;
    xpoint_eqw_fp_t K1_sw;
    point_eqw_fp_t phi_1_pts_eqw_sw[2 * MAX_STRATEGY_LENGTH];
    mg_to_sw_fp(&E_sw, &K1_sw, phi_1_pts_eqw_sw, E, &K1, 1, phi_1_pts_eqw, 2 * strategy_length);
        // printf(":: [Diagonal isogenies] (1) mg2sw took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

        // start = clock();
    curve_sw_fp_t E1_sw;
    fp_set_one(&E1_sw.A);
    fp_set_one(&E1_sw.B);
    fp_set_one(&E1_sw.C);
    point_eqw_fp_t phi_1_pts_mapped_eqw_sw[2 * MAX_STRATEGY_LENGTH];
    sw_eqw_2isog_chain_eval_fp(&E1_sw, phi_1_pts_mapped_eqw_sw, &E_sw, &K1_sw, sk->v, phi_1_pts_eqw_sw, 2 * strategy_length);
        // printf(":: [Diagonal isogenies] (1) Diagonal isogenies took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

        // start = clock();
    curve_mg_fp_t E1_m;
    point_eqw_fp_t phi_1_pts_mapped_eqw_m[2 * MAX_STRATEGY_LENGTH];
    xpoint_eqw_fp_t xT81;
    fp_copy(&xT81.x, &(phi_1_pts_mapped_eqw_sw + 2 * strategy_length - 2)->x);
    fp_copy(&xT81.z, &(phi_1_pts_mapped_eqw_sw + 2 * strategy_length - 2)->z);
    // Don't need to push any x-only points
    sw_to_mg_fp(&E1_m, &_, phi_1_pts_mapped_eqw_m, &E1_sw, &xT81, &__, 0, phi_1_pts_mapped_eqw_sw, 2 * strategy_length);
        // printf(":: [Diagonal isogenies] (1) sw2mg 1 took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

    // 2
        // start = clock();
    point_eqw_fp_t phi_2_pts_eqw[2 * MAX_STRATEGY_LENGTH];
    for (int i = 0; i < strategy_length; i++) {
        // i.e. "2**n_i * P2"
        point_eqw_fp_copy(phi_2_pts_eqw + 2 * i, &cached_doubles[i][1]);
        // i.e. "2**n_i * Q2"
        point_eqw_fp_copy(phi_2_pts_eqw + 2 * i + 1, &cached_doubles[i][3]);
        // The Q-points lie on the twist, we only map the x-coordinate
        // The y-coordinate is twisted after the diagonal isogenies have been
        // computed
        x_twist_i(phi_2_pts_eqw + 2 * i + 1);
    }
        // printf(":: [Diagonal isogenies] (2) Copying points took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

        // start = clock();
    curve_sw_fp_t Ep_sw;
    xpoint_eqw_fp_t K2_sw;
    point_eqw_fp_t phi_2_pts_eqw_sw[2 * MAX_STRATEGY_LENGTH];
    mg_to_sw_fp(&Ep_sw, &K2_sw, phi_2_pts_eqw_sw, E, &K2, 1, phi_2_pts_eqw, 2 * strategy_length);
        // printf(":: [Diagonal isogenies] (2) mg2sw took %.3Lf ms\n", (long double)((clock() - start) * 1000 ) / CLOCKS_PER_SEC);

        // start = clock();
    curve_sw_fp_t E2_sw;
    fp_set_one(&E2_sw.A);
    fp_set_one(&E2_sw.B);
    fp_set_one(&E2_sw.C);
    point_eqw_fp_t phi_2_pts_mapped_eqw_sw[2 * MAX_STRATEGY_LENGTH];
    sw_eqw_2isog_chain_eval_fp(&E2_sw, phi_2_pts_mapped_eqw_sw, &Ep_sw, &K2_sw, sk->v, phi_2_pts_eqw_sw, 2 * strategy_length);
        // printf(":: [Diagonal isogenies] (2) Diagonal isogenies took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

        // start = clock();
    curve_mg_fp_t E2_m;
    point_eqw_fp_t phi_2_pts_mapped_eqw_m[2 * MAX_STRATEGY_LENGTH];
    xpoint_eqw_fp_t xT82;
    fp_copy(&xT82.x, &(phi_2_pts_mapped_eqw_sw + 2 * strategy_length - 2)->x);
    fp_copy(&xT82.z, &(phi_2_pts_mapped_eqw_sw + 2 * strategy_length - 2)->z);
    // Don't need to map any x-points
    sw_to_mg_fp(&E2_m, &_, phi_2_pts_mapped_eqw_m, &E2_sw, &xT82, &__, 0, phi_2_pts_mapped_eqw_sw, 2 * strategy_length);
        // printf(":: [Diagonal isogenies] (2) sw2mg took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);
        // printf("=> [Diagonal isogenies] Total took %.3Lf ms\n", (long double)((clock() - start_diagonal_isogenies) * 1000) / CLOCKS_PER_SEC);

        // clock_t start_mapping_to_fp2 = clock();
        // start = clock();

    // curve_mg_fp_normalise(&E1_m);
    // curve_mg_fp_normalise(&E2_m);
    // fp_copy(&E12->E1.A, &E1_m.A);
    // fp_copy(&E12->E1.C, &E1_m.C);
    // fp_copy(&E12->E2.A, &E2_m.A);
    // fp_copy(&E12->E2.C, &E2_m.C);

    // curve_mg_fp_normalise_and_A24(&E1_m);
    // curve_mg_fp_normalise_and_A24(&E2_m);
    curve_mg_fp_normalise(&E1_m);
    curve_mg_fp_normalise(&E2_m);
    curve_mg_fp_copy(&E12->E1, &E1_m);
    curve_mg_fp_copy(&E12->E2, &E2_m);

        // printf("=> [Mapping kernel to jacobian] Normalisation took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

        // start = clock();
    for (int i = 0; i < strategy_length; i++) {
        point_eqw_to_jac_fp_to_fp2(&(C1_jac_dbls + i)->P1, phi_1_pts_mapped_eqw_m + 2 * i);
        point_eqw_to_jac_fp_to_fp2(&(C1_jac_dbls + i)->P2, phi_2_pts_mapped_eqw_m + 2 * i);
        point_eqw_to_jac_fp_to_fp2(&(C2_jac_dbls + i)->P1, phi_1_pts_mapped_eqw_m + 2 * i + 1);
        point_eqw_to_jac_fp_to_fp2(&(C2_jac_dbls + i)->P2, phi_2_pts_mapped_eqw_m + 2 * i + 1);
    }
        // printf("=> [Mapping kernel to jacobian] Mapping to jacobian took %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);
        // start = clock();
    for (int i = 0; i < strategy_length; i++) {
        // Finally, map the points on the second curve through "the" rest of the
        // twisting map
        y_twist_i(&(C2_jac_dbls + i)->P1);
        y_twist_i(&(C2_jac_dbls + i)->P2);
    }
        // printf("=> [Mapping kernel to jacobian] Twisting took %.3Lf ms\n", (long double)(clock() - start) * 1000 / CLOCKS_PER_SEC);
        // printf("=> [Mapping kernel to jacobian] Total took %.3Lf ms\n", (long double)((clock() - start_mapping_to_fp2) * 1000) / CLOCKS_PER_SEC);
}

uint32_t
coral_compute_hd(
    theta_couple_curve_fp_t *E34,
    theta_couple_curve_fp_t *E12,
    uint32_t chain_length,
    theta_couple_jac_point_fp2_t *C1_jac_dbls,
    theta_couple_jac_point_fp2_t *C2_jac_dbls,
    uint16_t *strategy,
    uint16_t strategy_length
) {
    return theta_chain_compute_fp2(E34, E12, chain_length, C1_jac_dbls, C2_jac_dbls, strategy, strategy_length);
}

uint32_t
coral_compute_action(
    curve_mg_fp_t *E_action,
    const coral_secret_key_t *sk,
    curve_mg_fp_t *E
) {
    // clock_t start;

    curve_mg_fp_t Et;
    curve_mg_fp_init(&Et);

        // start = clock();
    point_eqw_fp_t P, Q;
    curve_mg_fp_2e_basis(&P, &Q, &Et, E, sk->e + 2);
        // printf("=> [Basis sampling] Total time: %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

        // start = clock();
    // Compute the strategy so that we can cache doubles
    uint16_t strategy[MAX_STRATEGY_LENGTH];
    memset(strategy, 0, MAX_STRATEGY_LENGTH * sizeof(uint16_t));
    uint16_t strategy_length = modified_balanced_strategy(strategy, sk->e - (sk->v + 1));
        // printf("=> [Computing initial strategy] Total time: %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

    theta_couple_jac_point_fp2_t C1_jac_dbls[MAX_STRATEGY_LENGTH], C2_jac_dbls[MAX_STRATEGY_LENGTH];
    theta_couple_curve_fp_t E12, E34;

    curve_mg_fp_init(&E12.E1);
    curve_mg_fp_init(&E12.E2);
    curve_mg_fp_init(&E34.E1);
    curve_mg_fp_init(&E34.E2);

        // start = clock();
    coral_prepare_hd_kernel(C1_jac_dbls, C2_jac_dbls, &E12, strategy, strategy_length, sk, &P, E, &Q, &Et);
        // printf("=> [2d kernel preparation + diagonals] Total time: %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

    // (void) E34; return 0;
        // start = clock();
    uint32_t success = coral_compute_hd(&E34, &E12, sk->e - (sk->v + 1), C1_jac_dbls, C2_jac_dbls, strategy, strategy_length);
    curve_mg_fp_normalise(&(E34.E2));
    fp_neg(&E34.E2.A, &E34.E2.A);
    curve_mg_fp_copy(E_action, &E34.E2);
        // printf("=> [2d isogeny computation] Total time: %.3Lf ms\n", (long double)((clock() - start) * 1000) / CLOCKS_PER_SEC);

    return success;
}
