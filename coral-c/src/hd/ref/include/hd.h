// Author: Antonin Leroux
// Edited by the CORAL team for compatibility over Fp

#ifndef HD_H
#define HD_H

#include <assert.h>
#include <stdio.h>

#include "sqisign_namespace.h"
#include "ec.h"

#define HD_extra_torsion 2

// We can hardcode an upper limit on the length of strategy
// This (modified) balanced strategy needs at most
//     16 + log_2(chain_length) < 16 + log_2(log_2(p))
// entries
// For now, we hardcode upper-bound log_2(log_2(p)) by 16
// This could/should be made a prime-dependent pre-computed constant in the
// future to use less memory
// We add 1 to detect logic errors when computing the strategy
#define MAX_STRATEGY_LENGTH 33
uint16_t modified_balanced_strategy(uint16_t *strategy, uint32_t chain_length);

// *** Fp ***

typedef struct theta_couple_point_fp_t { xpoint_eqw_fp_t P1; xpoint_eqw_fp_t P2; } theta_couple_point_fp_t;

// T1m2 = T1 - T2
typedef struct theta_couple_jac_point_fp_t { point_jac_fp_t P1; point_jac_fp_t P2; } theta_couple_jac_point_fp_t;
typedef struct theta_kernel_jac_couple_points_fp_t { theta_couple_jac_point_fp_t T1; theta_couple_jac_point_fp_t T2; } theta_kernel_jac_couple_points_fp_t;
typedef struct theta_couple_curve_fp_t { curve_mg_fp_t E1; curve_mg_fp_t E2; } theta_couple_curve_fp_t;

// Bi 2^*-torsion basis on Ei
typedef struct theta_point_fp_t { fp_t x; fp_t y; fp_t z; fp_t t; } theta_point_fp_t;
// Theta point with repeated components
typedef struct theta_point_compact_fp_t { fp_t x; fp_t y; } theta_point_compact_fp_t;

typedef struct theta_structure_fp_t
{
    theta_point_fp_t null_point;
    bool precomputation;
    // Precomputed values used for doubling and (2, 2)-isogenies
    fp_t XYZ0, YZT0, XZT0, XYT0;
    fp_t xyz0, yzt0, xzt0, xyt0;
} theta_structure_fp_t;

typedef struct translation_matrix_fp_t { fp_t g00; fp_t g01; fp_t g10; fp_t g11; } translation_matrix_fp_t;
typedef struct basis_change_matrix_fp_t { fp_t m[4][4]; } basis_change_matrix_fp_t;

typedef struct theta_gluing_fp_t
{
    theta_couple_curve_fp_t domain;
    theta_couple_jac_point_fp_t xyK1_8;
    theta_point_compact_fp_t imageK1_8;
    basis_change_matrix_fp_t M;
    theta_point_fp_t precomputation;
    theta_point_fp_t codomain;
} theta_gluing_fp_t;

typedef struct theta_isogeny_fp_t
{
    theta_point_fp_t T1_8;
    theta_point_fp_t T2_8;
    bool hadamard_bool_1;
    bool hadamard_bool_2;
    theta_structure_fp_t domain;
    theta_point_fp_t precomputation;
    theta_structure_fp_t codomain;
} theta_isogeny_fp_t;

typedef struct theta_splitting_fp_t { basis_change_matrix_fp_t M; theta_structure_fp_t B; } theta_splitting_fp_t;

void double_couple_point_fp(theta_couple_point_fp_t *out, const theta_couple_point_fp_t *in, const theta_couple_curve_fp_t *E1E2);

void double_couple_jac_point_fp(
    theta_couple_jac_point_fp_t *out,
    const theta_couple_jac_point_fp_t *in,
    const theta_couple_curve_fp_t *E1E2
);
void double_couple_jac_point_iter_fp(
    theta_couple_jac_point_fp_t *out,
    unsigned n,
    const theta_couple_jac_point_fp_t *in,
    const theta_couple_curve_fp_t *E1E2
);

// Map individual xyz jac points to x-only equally weighted
void couple_point_jac_to_ec_fp(theta_couple_point_fp_t *P, const theta_couple_jac_point_fp_t *xyP);

/**
 * @brief Compute a (2,2) isogeny chain in dimension 2 between elliptic products
 * in the theta_model
 * Returns 1 on success, and 0 else
 *
 * Evaluates phi: E1 x E2 -> E3 x E4 with kernel <((P1, Q1), (P2, Q2))>
 *
 * We pass the kernel along with the first set of doublings down to the gluing
 * kernel
 *
 * @param [OUTPUT] E34: The codomain product
 * @param [INPUT]  E12: The domain product
 * @param [INPUT]  chain_length: Length of (2, 2)-chain
 * @param [INPUT]  C1_jac_dbls: Pre-computed doubles of the first couple point (P1, Q1)
 * @param [INPUT]  C2_jac_dbls: Pre-computed doubles of the second couple point (P2, Q2)
 * @param [INPUT]  strategy_length: Number of pre-computed doubles (i.e. length of initial strategy)
 * @returns 1 on success 0 on failure
 *
 */
uint32_t
theta_chain_compute_fp(
    theta_couple_curve_fp_t *E34,
    theta_couple_curve_fp_t *E12,
    const uint32_t chain_length,
    theta_couple_jac_point_fp_t *C1_jac_dbls,
    theta_couple_jac_point_fp_t *C2_jac_dbls,
    uint16_t *strategy,
    const uint16_t strategy_length
);


// *** Fp2 ***
typedef struct theta_couple_point_fp2_t { xpoint_eqw_fp2_t P1; xpoint_eqw_fp2_t P2; } theta_couple_point_fp2_t;

// T1m2 = T1 - T2
typedef struct theta_couple_jac_point_fp2_t { point_jac_fp2_t P1; point_jac_fp2_t P2; } theta_couple_jac_point_fp2_t;
typedef struct theta_kernel_jac_couple_points_fp2_t { theta_couple_jac_point_fp2_t T1; theta_couple_jac_point_fp2_t T2; } theta_kernel_jac_couple_points_fp2_t;
typedef struct theta_couple_curve_fp2_t { curve_mg_fp2_t E1; curve_mg_fp2_t E2; } theta_couple_curve_fp2_t;

// Bi 2^*-torsion basis on Ei
typedef struct theta_point_fp2_t { fp2_t x; fp2_t y; fp2_t z; fp2_t t; } theta_point_fp2_t;

static inline void
theta_point_fp2_init(theta_point_fp2_t *theta_point) {
    fp2_set_zero(&theta_point->x);
    fp2_set_zero(&theta_point->y);
    fp2_set_zero(&theta_point->z);
    fp2_set_zero(&theta_point->t);
}

// Theta point with repeated components
typedef struct theta_point_compact_fp2_t { fp2_t x; fp2_t y; } theta_point_compact_fp2_t;

typedef struct theta_structure_fp2_t
{
    theta_point_fp2_t null_point;
    bool precomputation;
    // Precomputed values used for doubling and (2, 2)-isogenies
    fp2_t XYZ0, YZT0, XZT0, XYT0;
    fp2_t xyz0, yzt0, xzt0, xyt0;
} theta_structure_fp2_t;

typedef struct translation_matrix_fp2_t { fp2_t g00; fp2_t g01; fp2_t g10; fp2_t g11; } translation_matrix_fp2_t;
typedef struct basis_change_matrix_fp2_t { fp2_t m[4][4]; } basis_change_matrix_fp2_t;

typedef struct theta_gluing_fp2_t
{
    theta_couple_curve_fp2_t domain;
    theta_couple_jac_point_fp2_t xyK1_8;
    theta_point_compact_fp2_t imageK1_8;
    basis_change_matrix_fp2_t M;
    theta_point_fp2_t precomputation;
    theta_point_fp2_t codomain;
} theta_gluing_fp2_t;

typedef struct theta_isogeny_fp2_t
{
    theta_point_fp2_t T1_8;
    theta_point_fp2_t T2_8;
    bool hadamard_bool_1;
    bool hadamard_bool_2;
    theta_structure_fp2_t domain;
    theta_point_fp2_t precomputation;
    theta_structure_fp2_t codomain;
} theta_isogeny_fp2_t;

typedef struct theta_splitting_fp2_t { basis_change_matrix_fp2_t M; theta_structure_fp2_t B; } theta_splitting_fp2_t;

void double_couple_point_fp2(theta_couple_point_fp2_t *out, const theta_couple_point_fp2_t *in, const theta_couple_curve_fp2_t *E1E2);

void double_couple_jac_point_fp2(
    theta_couple_jac_point_fp2_t *out,
    const theta_couple_jac_point_fp2_t *in,
    const theta_couple_curve_fp2_t *E1E2
);
void double_couple_jac_point_iter_fp2(
    theta_couple_jac_point_fp2_t *out,
    unsigned n,
    const theta_couple_jac_point_fp2_t *in,
    const theta_couple_curve_fp2_t *E1E2
);

// Map individual xyz jac points to x-only equally weighted
void couple_point_jac_to_ec_fp2(theta_couple_point_fp2_t *P, const theta_couple_jac_point_fp2_t *xyP);

/**
 * @brief Compute a (2,2) isogeny chain in dimension 2 between elliptic products
 * in the theta_model
 * Returns 1 on success, and 0 else
 *
 * Evaluates phi: E1 x E2 -> E3 x E4 with kernel <((P1, Q1), (P2, Q2))>
 *
 * We pass the kernel along with the first set of doublings down to the gluing
 * kernel
 *
 * @param [OUTPUT] E34: The codomain product
 * @param [INPUT]  E12: The domain product
 * @param [INPUT]  chain_length: Length of (2, 2)-chain
 * @param [INPUT]  C1_jac_dbls: Pre-computed doubles of the first couple point (P1, Q1)
 * @param [INPUT]  C2_jac_dbls: Pre-computed doubles of the second couple point (P2, Q2)
 * @param [INPUT]  strategy_length: Number of pre-computed doubles (i.e. length of initial strategy)
 * @returns 1 on success 0 on failure
 *
 */
uint32_t
theta_chain_compute_fp2(
    theta_couple_curve_fp_t *E34,
    theta_couple_curve_fp_t *E12,
    const uint32_t chain_length,
    theta_couple_jac_point_fp2_t *C1_jac_dbls,
    theta_couple_jac_point_fp2_t *C2_jac_dbls,
    uint16_t *strategy,
    const uint16_t strategy_length
);

// *** Conversions ***

static
void
theta_point_fp2_to_fp(
    theta_point_fp_t *theta_point_fp,
    theta_point_fp2_t *theta_point_fp2
) {
    if(!fp_is_zero(&theta_point_fp2->x.im)) {
        fp2_print("[ERROR] Not over Fp: x =", &theta_point_fp2->x);
        exit(1);
    }
    if(!fp_is_zero(&theta_point_fp2->y.im)) {
        fp2_print("[ERROR] Not over Fp: y =", &theta_point_fp2->y);
        exit(1);
    }
    if(!fp_is_zero(&theta_point_fp2->z.im)) {
        fp2_print("[ERROR] Not over Fp: z = ", &theta_point_fp2->z);
        exit(1);
    }
    if(!fp_is_zero(&theta_point_fp2->t.im)) {
        fp2_print("[ERROR] Not over Fp: t = ", &theta_point_fp2->t);
        exit(1);
    }

    assert(fp_is_zero(&theta_point_fp2->x.im));
    assert(fp_is_zero(&theta_point_fp2->y.im));
    assert(fp_is_zero(&theta_point_fp2->z.im));
    assert(fp_is_zero(&theta_point_fp2->t.im));

    fp_copy(&theta_point_fp->x, &theta_point_fp2->x.re);
    fp_copy(&theta_point_fp->y, &theta_point_fp2->y.re);
    fp_copy(&theta_point_fp->z, &theta_point_fp2->z.re);
    fp_copy(&theta_point_fp->t, &theta_point_fp2->t.re);
}

static
void
theta_point_fp_to_fp2(
    theta_point_fp2_t *theta_point_fp2,
    theta_point_fp_t *theta_point_fp
) {
    fp_copy(&theta_point_fp2->x.re, &theta_point_fp->x);
    fp_copy(&theta_point_fp2->y.re, &theta_point_fp->y);
    fp_copy(&theta_point_fp2->z.re, &theta_point_fp->z);
    fp_copy(&theta_point_fp2->t.re, &theta_point_fp->t);

    fp_set_zero(&theta_point_fp2->x.im);
    fp_set_zero(&theta_point_fp2->y.im);
    fp_set_zero(&theta_point_fp2->z.im);
    fp_set_zero(&theta_point_fp2->t.im);
}

static
void
theta_structure_fp2_to_fp(
    theta_structure_fp_t *theta_structure_fp,
    theta_structure_fp2_t *theta_structure_fp2
) {
    assert(fp_is_zero(&theta_structure_fp2->XYZ0.im));
    assert(fp_is_zero(&theta_structure_fp2->YZT0.im));
    assert(fp_is_zero(&theta_structure_fp2->XZT0.im));
    assert(fp_is_zero(&theta_structure_fp2->XYT0.im));

    assert(fp_is_zero(&theta_structure_fp2->xyz0.im));
    assert(fp_is_zero(&theta_structure_fp2->yzt0.im));
    assert(fp_is_zero(&theta_structure_fp2->xzt0.im));
    assert(fp_is_zero(&theta_structure_fp2->xyt0.im));

    fp_copy(&theta_structure_fp->XYZ0, &theta_structure_fp2->XYZ0.re);
    fp_copy(&theta_structure_fp->YZT0, &theta_structure_fp2->YZT0.re);
    fp_copy(&theta_structure_fp->XZT0, &theta_structure_fp2->XZT0.re);
    fp_copy(&theta_structure_fp->XYT0, &theta_structure_fp2->XYT0.re);

    fp_copy(&theta_structure_fp->xyz0, &theta_structure_fp2->xyz0.re);
    fp_copy(&theta_structure_fp->yzt0, &theta_structure_fp2->yzt0.re);
    fp_copy(&theta_structure_fp->xzt0, &theta_structure_fp2->xzt0.re);
    fp_copy(&theta_structure_fp->xyt0, &theta_structure_fp2->xyt0.re);
}

static
void
theta_structure_fp_to_fp2(
    theta_structure_fp2_t *theta_structure_fp2,
    theta_structure_fp_t *theta_structure_fp
) {
    fp_copy(&theta_structure_fp2->XYZ0.re, &theta_structure_fp->XYZ0);
    fp_copy(&theta_structure_fp2->YZT0.re, &theta_structure_fp->YZT0);
    fp_copy(&theta_structure_fp2->XZT0.re, &theta_structure_fp->XZT0);
    fp_copy(&theta_structure_fp2->XYT0.re, &theta_structure_fp->XYT0);

    fp_copy(&theta_structure_fp2->xyz0.re, &theta_structure_fp->xyz0);
    fp_copy(&theta_structure_fp2->yzt0.re, &theta_structure_fp->yzt0);
    fp_copy(&theta_structure_fp2->xzt0.re, &theta_structure_fp->xzt0);
    fp_copy(&theta_structure_fp2->xyt0.re, &theta_structure_fp->xyt0);

    fp_set_zero(&(theta_structure_fp2->XYZ0).im);
    fp_set_zero(&(theta_structure_fp2->YZT0).im);
    fp_set_zero(&(theta_structure_fp2->XZT0).im);
    fp_set_zero(&(theta_structure_fp2->XYT0).im);

    fp_set_zero(&(theta_structure_fp2->xyz0).im);
    fp_set_zero(&(theta_structure_fp2->yzt0).im);
    fp_set_zero(&(theta_structure_fp2->xzt0).im);
    fp_set_zero(&(theta_structure_fp2->xyt0).im);
}

void choose_index_theta_point_fp(fp_t *res, int ind, const theta_point_fp_t *T);
void apply_isomorphism_general_fp(theta_point_fp_t *res, const basis_change_matrix_fp_t *M, const theta_point_fp_t *P, const bool Pt_not_zero);
void apply_isomorphism_fp(theta_point_fp_t *res, const basis_change_matrix_fp_t *M, const theta_point_fp_t *P);
void base_change_fp(theta_point_fp_t *out, const theta_gluing_fp_t *phi, const theta_couple_point_fp_t *T);
void action_by_translation_z_and_det_fp(fp_t *z_inv, fp_t *det_inv, const xpoint_eqw_fp_t *P4, const xpoint_eqw_fp_t *P2);
void action_by_translation_compute_matrix_fp(translation_matrix_fp_t *G, const xpoint_eqw_fp_t *P4, const xpoint_eqw_fp_t *P2, const fp_t *z_inv, const fp_t *det_inv);
int verify_two_torsion_fp(const theta_couple_point_fp_t *K1_2, const theta_couple_point_fp_t *K2_2, const theta_couple_curve_fp_t *E12);
int action_by_translation_fp(translation_matrix_fp_t *Gi, const theta_couple_point_fp_t *K1_4, const theta_couple_point_fp_t *K2_4, const theta_couple_curve_fp_t *E12);
int gluing_change_of_basis_fp(basis_change_matrix_fp_t *M, const theta_couple_point_fp_t *K1_4, const theta_couple_point_fp_t *K2_4, const theta_couple_curve_fp_t *E12);

int theta_isogeny_fp_compute(theta_isogeny_fp_t *out, const theta_structure_fp_t *A, const theta_point_fp_t *T1_8, const theta_point_fp_t *T2_8, bool hadamard_bool_1, bool hadamard_bool_2, bool verify);
void theta_isogeny_fp_compute_4(theta_isogeny_fp_t *out, const theta_structure_fp_t *A, const theta_point_fp_t *T1_4, const theta_point_fp_t *T2_4, bool hadamard_bool_1, bool hadamard_bool_2);
void theta_isogeny_fp_compute_2(theta_isogeny_fp_t *out, const theta_structure_fp_t *A, const theta_point_fp_t *T1_2, const theta_point_fp_t *T2_2, bool hadamard_bool_1, bool hadamard_bool_2);
void theta_isogeny_fp_eval(theta_point_fp_t *out, const theta_isogeny_fp_t *phi, const theta_point_fp_t *P);

void double_couple_jac_point_iter_fp2(
    theta_couple_jac_point_fp2_t *out,
    unsigned n,
    const theta_couple_jac_point_fp2_t *in,
    const theta_couple_curve_fp2_t *E1E2
);

#endif
