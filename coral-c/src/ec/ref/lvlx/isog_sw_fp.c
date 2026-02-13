#include <assert.h>

#include "ec.h"


void
sw_xdouble_fp(
        xpoint_eqw_fp_t *two_P,
        const curve_sw_fp_t *E,
        const fp_t *C_pow2,
        const fp_t *C_pow3,
        const xpoint_eqw_fp_t *P
) {
    // Double xz-point `Pxz` on the curve E: y^2 = x^3 + (a/c^2)x + (b/c^3)
    //
    // Inplace operation is permitted (i.e. two_P, P point to same memory)
    //
    // We demand that the curve constants (c_pow2, c_pow3) are pre-computed,
    // because it is often the case that multiple points need to be doubled on
    // the same curve
    //
    // Rrojective implementation of the usual (affine) formula
    // x = (3x^2 + a)^2 / 4 / (x^3 + ax + b) - 2x
    // Must replace x <- x/z, a <- A/C^2, b <- B/C^3

    fp_t Px_pow2, Px_pow3;
    fp_sqr(&Px_pow2, &P->x);
    fp_mul(&Px_pow3, &Px_pow2, &P->x);

    fp_t Pz_pow2, Pz_pow3;
    fp_sqr(&Pz_pow2, &P->z);
    fp_mul(&Pz_pow3, &Pz_pow2, &P->z);

    fp_t A__Pz_pow2;
    fp_mul(&A__Pz_pow2, &E->A, &Pz_pow2);

    // Numerator of m^2 = (3x^2 + a)^2
    // m2_n = (3 * (Px_pow2 * C_pow2) + A * Pz_pow2)**2
    fp_t m2_n, tmp1;
    fp_mul(&m2_n, &Px_pow2, C_pow2);
    // fp_mul_small(&m2_n, &m2_n, 3);
        fp_add(&tmp1, &m2_n, &m2_n);
        fp_add(&m2_n, &m2_n, &tmp1);
    fp_add(&m2_n, &m2_n, &A__Pz_pow2);
    fp_sqr(&m2_n, &m2_n);

    // Denominator of m^2 = (3x^2 + a)^2
    // m2_d = 4 * (Px_pow3 * C_pow3 + C * Px * A * Pz_pow2 + B * Pz_pow3)
    fp_t m2_d, tmp2;
    fp_mul(&m2_d, &Px_pow3, C_pow3);
    fp_mul(&tmp1, &E->C, &E->A);
    fp_mul(&tmp1, &tmp1, &P->x);
    fp_mul(&tmp1, &tmp1, &Pz_pow2);
    fp_mul(&tmp2, &E->B, &Pz_pow3);
    fp_add(&m2_d, &m2_d, &tmp1);
    fp_add(&m2_d, &m2_d, &tmp2);
    // fp_mul_small(&m2_d, &m2_d, 4);
        fp_add(&m2_d, &m2_d, &m2_d);
        fp_add(&m2_d, &m2_d, &m2_d);

    // (2P)x = m2_n - 2 * (Px * m2_d * C)
    // (2P)z = Pz * m2_d * C
    // Note to future implemetors (potential footgun) : Be careful with ordering
    // of operations to continue allowing inplace operation
    fp_mul(&two_P->x, &P->x, &m2_d);
    fp_mul(&two_P->x, &two_P->x, &E->C);
    fp_add(&two_P->x, &two_P->x, &two_P->x);
    fp_sub(&two_P->x, &m2_n, &two_P->x);

    fp_mul(&two_P->z, &P->z, &m2_d);
    fp_mul(&two_P->z, &two_P->z, &E->C);
}


void
sw_eqw_2isog_eval_fp(
    point_eqw_fp_t *Q,
    const xpoint_eqw_fp_t *K,
    const point_eqw_fp_t *P,
    const fp_t *T,
    const fp_t *c_pow2
) {
    // Cost: 10M + 2S + 2s + 1a
    point_eqw_fp_t R;

    // tmp1 = Px * Kz - Kx * Pz
    // tmp2 = tmp1**2 * c_pow2
    // tmp3 = Pz**2 * T
    // Qx = Kz * Px * tmp2 + tmp3 * tmp1
    // Qy = (tmp2 - tmp3) * Py * Kz
    // Qz = Pz * Kz * tmp2

    fp_t tmp1, tmp2, tmp3;
    // tmp1 = Px * Kz - Pz * Kx
    fp_mul(&tmp1, &P->x, &K->z);
    fp_mul(&tmp2, &P->z, &K->x);
    fp_sub(&tmp1, &tmp1, &tmp2);

    // tmp2 = (Px * Kz - Pz * Kx) ** 2 * C_pow2
    fp_sqr(&tmp2, &tmp1);
    fp_mul(&tmp2, &tmp2, c_pow2);

    // tmp3 = Pz^2 * T
    fp_sqr(&tmp3, &P->z);
    fp_mul(&tmp3, &tmp3, T);

    fp_t tmp4;
    fp_mul(&R.x, &K->z, &P->x);
    fp_mul(&R.x, &R.x, &tmp2);
    fp_mul(&tmp4, &tmp3, &tmp1);
    fp_add(&R.x, &R.x, &tmp4);

    fp_sub(&R.y, &tmp2, &tmp3);
    fp_mul(&R.y, &R.y, &P->y);
    fp_mul(&R.y, &R.y, &K->z);

    fp_mul(&R.z, &P->z, &K->z);
    fp_mul(&R.z, &R.z, &tmp2);

    point_eqw_fp_copy(Q, &R);
}


void
sw_eqw_2isog_xeval_fp(
    xpoint_eqw_fp_t *Q,
    const xpoint_eqw_fp_t *K,
    const xpoint_eqw_fp_t *P,
    const fp_t *T,
    const fp_t *c_pow2
) {
    // Cost: 7M + 1S + 1s + 1a
    xpoint_eqw_fp_t R;

    // tmp1 = Kz * (Px * Kz - Kx * Pz) * c_pow2
    // tmp2 = Pz**2 * T
    // Rx = Px * tmp1 + tmp2
    // Rz = Pz * tmp1

    fp_t tmp1, tmp2;
    // tmp1 = (Px * Kz - Pz * Kx) * Kz * c_pow2
    fp_mul(&tmp1, &P->x, &K->z);
    fp_mul(&tmp2, &P->z, &K->x);
    fp_sub(&tmp1, &tmp1, &tmp2);
    fp_mul(&tmp1, &tmp1, &K->z);
    fp_mul(&tmp1, &tmp1, c_pow2);

    // tmp2 = Pz^2 * T
    fp_sqr(&tmp2, &P->z);
    fp_mul(&tmp2, &tmp2, T);

    // Rx = Px * tmp1 + tmp2
    // Rz = Pz * tmp1
    fp_mul(&R.x, &P->x, &tmp1);
    fp_add(&R.x, &R.x, &tmp2);
    fp_mul(&R.z, &P->z, &tmp1);

    xpoint_eqw_fp_copy(Q, &R);
}


void
sw_eqw_2isog_chain_eval_fp(
    curve_sw_fp_t *E2,
    point_eqw_fp_t *E2_pts,
    const curve_sw_fp_t *E1,
    const xpoint_eqw_fp_t *_K,
    const uint32_t log_2_K_order,
    const point_eqw_fp_t *E1_pts,
    const uint32_t len_pts
) {
    fp_copy(&E2->A, &E1->A);
    fp_copy(&E2->B, &E1->B);
    fp_copy(&E2->C, &E1->C);

    // This function does not support inplace operation
    assert (E1 != E2);
    assert (E1_pts != E2_pts);

    for (uint32_t i = 0; i < len_pts; i++)
        point_eqw_fp_copy(E2_pts + i, E1_pts + i);

    fp_t C_pow2, C_pow3, T, W;

    xpoint_eqw_fp_t K, k;
    xpoint_eqw_fp_copy(&K, _K);

    for (uint32_t order = log_2_K_order; order > 0; order--) {
        fp_sqr(&C_pow2, &E2->C);
        fp_mul(&C_pow3, &C_pow2, &E2->C);

        // Scale point down to order 2
        xpoint_eqw_fp_copy(&k, &K);
        for (uint32_t _ = 0; _ < order - 1; _++)
            sw_xdouble_fp(&k, E2, &C_pow2, &C_pow3, &k);

        xpoint_eqw_fp_t _k;
        xpoint_eqw_fp_copy(&_k, &k);

        // Recall t = 3 * (Kx / Kz)**2 + a, define T := C**2 * kz**2 * t
        // So T = 3 * C**2 * Kx**2 + A * Kz**2
        // T = C_pow2 * kx**2
        // T = T + T + T
        // T = T + A * kz**2

        fp_t tmp;
        fp_sqr(&T, &k.x);
        fp_mul(&T, &T, &C_pow2);
        // fp_mul_small(&T, &T, 3);
            fp_add(&tmp, &T, &T);
            fp_add(&T, &T, &tmp);
        fp_sqr(&tmp, &k.z);
        fp_mul(&tmp, &tmp, &E2->A);
        fp_add(&T, &T, &tmp);

        if (order > 1)
            sw_eqw_2isog_xeval_fp(&K, &k, &K, &T, &C_pow2);

        for (uint32_t i = 0; i < len_pts; i++) {
            point_eqw_fp_t tmp;
            sw_eqw_2isog_eval_fp(&tmp, &k, E2_pts + i, &T, &C_pow2);
            point_eqw_fp_copy(E2_pts + i, &tmp);
        }

        fp_t T_mul5;
        // fp_mul_small(&T_mul5, &T, 5);
            fp_add(&T_mul5, &T, &T);
            fp_add(&T_mul5, &T_mul5, &T_mul5);
            fp_add(&T_mul5, &T_mul5, &T);

        // Recall w = (Kx / Kz) * t, define W := C**2 * Kz**3 * w
        fp_mul(&W, &k.x, &T);

        fp_t WC, WC_mul7;
        fp_mul(&WC, &W, &E2->C);
        // fp_mul_small(&WC_mul7, &WC, 7);
            fp_add(&WC_mul7, &WC, &WC);
            fp_add(&WC_mul7, &WC_mul7, &WC_mul7);
            fp_add(&WC_mul7, &WC_mul7, &WC);
            fp_add(&WC_mul7, &WC_mul7, &WC);
            fp_add(&WC_mul7, &WC_mul7, &WC);

        fp_t kz_pow2, kz_pow3;
        fp_sqr(&kz_pow2, &k.z);
        fp_mul(&kz_pow3, &kz_pow2, &k.z);

        // A = A * kz_pow2 - T_mul5
        // B = B * kz_pow3 - WC_mul7
        // C = C * kz
        fp_mul(&E2->A, &E2->A, &kz_pow2);
        fp_sub(&E2->A, &E2->A, &T_mul5);
        fp_mul(&E2->B, &E2->B, &kz_pow3);
        fp_sub(&E2->B, &E2->B, &WC_mul7);
        fp_mul(&E2->C, &E2->C, &k.z);
    }
}
