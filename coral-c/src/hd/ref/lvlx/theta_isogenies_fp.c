// Author: Antonin Leroux

#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "inttypes.h"
#include "rng.h"
#include "theta_isogenies.h"
#include "tools.h"

// // Select a base change matrix in constant time, with M1 a regular
// // base change matrix and M2 a precomputed base change matrix
// // If option = 0 then M <- M1, else if option = 0xFF...FF then M <- M2
// inline void
// select_base_change_matrix(basis_change_matrix_fp_t *M,
//                           const basis_change_matrix_fp_t *M1,
//                           const precomp_basis_change_matrix_t *M2,
//                           const uint32_t option)
// {
//     for (int i = 0; i < 4; i++)
//         for (int j = 0; j < 4; j++)
//             fp_select(&M->m[i][j], &M1->m[i][j], &FP2_CONSTANTS[M2->m[i][j]], option);
// }

// // Set a regular base change matrix from a precomputed one
// inline void
// set_base_change_matrix_from_precomp(basis_change_matrix_fp_t *res, const precomp_basis_change_matrix_t *M)
// {
//     for (int i = 0; i < 4; i++)
//         for (int j = 0; j < 4; j++)
//             res->m[i][j] = FP2_CONSTANTS[M->m[i][j]];
// }

void
choose_index_theta_point_fp(fp_t *res, int ind, const theta_point_fp_t *T)
{
    const fp_t *src = NULL;
    switch (ind % 4) {
        case 0:
            src = &T->x;
            break;
        case 1:
            src = &T->y;
            break;
        case 2:
            src = &T->z;
            break;
        case 3:
            src = &T->t;
            break;
        default:
            assert(0);
    }
    fp_copy(res, src);
}

// same as apply_isomorphism_fp method but more efficient when the t component of P is zero.
void
apply_isomorphism_general_fp(theta_point_fp_t *res,
                          const basis_change_matrix_fp_t *M,
                          const theta_point_fp_t *P,
                          const bool Pt_not_zero)
{
    fp_t x1;
    theta_point_fp_t temp;

    fp_mul(&temp.x, &P->x, &M->m[0][0]);
    fp_mul(&x1, &P->y, &M->m[0][1]);
    fp_add(&temp.x, &temp.x, &x1);
    fp_mul(&x1, &P->z, &M->m[0][2]);
    fp_add(&temp.x, &temp.x, &x1);

    fp_mul(&temp.y, &P->x, &M->m[1][0]);
    fp_mul(&x1, &P->y, &M->m[1][1]);
    fp_add(&temp.y, &temp.y, &x1);
    fp_mul(&x1, &P->z, &M->m[1][2]);
    fp_add(&temp.y, &temp.y, &x1);

    fp_mul(&temp.z, &P->x, &M->m[2][0]);
    fp_mul(&x1, &P->y, &M->m[2][1]);
    fp_add(&temp.z, &temp.z, &x1);
    fp_mul(&x1, &P->z, &M->m[2][2]);
    fp_add(&temp.z, &temp.z, &x1);

    fp_mul(&temp.t, &P->x, &M->m[3][0]);
    fp_mul(&x1, &P->y, &M->m[3][1]);
    fp_add(&temp.t, &temp.t, &x1);
    fp_mul(&x1, &P->z, &M->m[3][2]);
    fp_add(&temp.t, &temp.t, &x1);

    if (Pt_not_zero) {
        fp_mul(&x1, &P->t, &M->m[0][3]);
        fp_add(&temp.x, &temp.x, &x1);

        fp_mul(&x1, &P->t, &M->m[1][3]);
        fp_add(&temp.y, &temp.y, &x1);

        fp_mul(&x1, &P->t, &M->m[2][3]);
        fp_add(&temp.z, &temp.z, &x1);

        fp_mul(&x1, &P->t, &M->m[3][3]);
        fp_add(&temp.t, &temp.t, &x1);
    }

    fp_copy(&res->x, &temp.x);
    fp_copy(&res->y, &temp.y);
    fp_copy(&res->z, &temp.z);
    fp_copy(&res->t, &temp.t);
}

void
apply_isomorphism_fp(theta_point_fp_t *res, const basis_change_matrix_fp_t *M, const theta_point_fp_t *P)
{
    apply_isomorphism_general_fp(res, M, P, true);
}

// // set res = M1 * M2 with matrix multiplication
// void
// base_change_matrix_multiplication(basis_change_matrix_fp_t *res,
//                                   const basis_change_matrix_fp_t *M1,
//                                   const basis_change_matrix_fp_t *M2)
// {
//     basis_change_matrix_fp_t tmp;
//     fp_t sum, m_ik, m_kj;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             fp_set_zero(&sum);
//             for (int k = 0; k < 4; k++) {
//                 m_ik = M1->m[i][k];
//                 m_kj = M2->m[k][j];
//                 fp_mul(&m_ik, &m_ik, &m_kj);
//                 fp_add(&sum, &sum, &m_ik);
//             }
//             tmp.m[i][j] = sum;
//         }
//     }
//     *res = tmp;
// }

// compute the theta_point_fp_t corresponding to the couple of point T on an elliptic product
void
base_change_fp(theta_point_fp_t *out, const theta_gluing_fp_t *phi, const theta_couple_point_fp_t *T)
{
    theta_point_fp_t null_point;

    // null_point = (a : b : c : d)
    // a = P1.x P2.x, b = P1.x P2.z, c = P1.z P2.x, d = P1.z P2.z
    fp_mul(&null_point.x, &T->P1.x, &T->P2.x);
    fp_mul(&null_point.y, &T->P1.x, &T->P2.z);
    fp_mul(&null_point.z, &T->P2.x, &T->P1.z);
    fp_mul(&null_point.t, &T->P1.z, &T->P2.z);

    // Apply the basis change
    apply_isomorphism_fp(out, &phi->M, &null_point);
}

void
action_by_translation_z_and_det_fp(fp_t *z_inv, fp_t *det_inv, const xpoint_eqw_fp_t *P4, const xpoint_eqw_fp_t *P2)
{
    // Store the Z-coordinate to invert
    fp_copy(z_inv, &P4->z);

    // Then collect detij = xij wij - uij zij
    fp_t tmp;
    fp_mul(det_inv, &P4->x, &P2->z);
    fp_mul(&tmp, &P4->z, &P2->x);
    fp_sub(det_inv, det_inv, &tmp);
}

void
action_by_translation_compute_matrix_fp(translation_matrix_fp_t *G,
                                     const xpoint_eqw_fp_t *P4,
                                     const xpoint_eqw_fp_t *P2,
                                     const fp_t *z_inv,
                                     const fp_t *det_inv)
{
    fp_t tmp;

    // Gi.g10 = uij xij /detij - xij/zij
    fp_mul(&tmp, &P4->x, z_inv);
    fp_mul(&G->g10, &P4->x, &P2->x);
    fp_mul(&G->g10, &G->g10, det_inv);
    fp_sub(&G->g10, &G->g10, &tmp);

    // Gi.g11 = uij zij * detij
    fp_mul(&G->g11, &P2->x, det_inv);
    fp_mul(&G->g11, &G->g11, &P4->z);

    // Gi.g00 = -Gi.g11
    fp_neg(&G->g00, &G->g11);

    // Gi.g01 = - wij zij detij
    fp_mul(&G->g01, &P2->z, det_inv);
    fp_mul(&G->g01, &G->g01, &P4->z);
    fp_neg(&G->g01, &G->g01);
}

// Returns 1 if the basis is as expected and 0 otherwise
// We only expect this to fail for malformed signatures, so
// do not require this to run in constant time.
int
verify_two_torsion_fp(const theta_couple_point_fp_t *K1_2, const theta_couple_point_fp_t *K2_2, const theta_couple_curve_fp_t *E12)
{
    // First check if any point in K1_2 or K2_2 is zero, if they are then the points did not have
    // order 8 when we started gluing
    if (xpoint_eqw_fp_is_zero(&K1_2->P1) | xpoint_eqw_fp_is_zero(&K1_2->P2) | xpoint_eqw_fp_is_zero(&K2_2->P1) | xpoint_eqw_fp_is_zero(&K2_2->P2)) {
        return 0;
    }

    // Now ensure that P1, Q1 and P2, Q2 are independent. For points of order two this means
    // that they're not the same
    if (xpoint_eqw_fp_is_equal(&K1_2->P1, &K2_2->P1) | xpoint_eqw_fp_is_equal(&K1_2->P2, &K2_2->P2)) {
        return 0;
    }

    // Finally, double points to ensure all points have order exactly 0
    theta_couple_point_fp_t O1, O2;
    double_couple_point_fp(&O1, K1_2, E12);
    double_couple_point_fp(&O2, K2_2, E12);
    // If this check fails then the points had order 2*f for some f, and the kernel is malformed.
    if (!(xpoint_eqw_fp_is_zero(&O1.P1) & xpoint_eqw_fp_is_zero(&O1.P2) & xpoint_eqw_fp_is_zero(&O2.P1) & xpoint_eqw_fp_is_zero(&O2.P2))) {
        return 0;
    }

    return 1;
}

// Computes the action by translation for four points
// (P1, P2) and (Q1, Q2) on E1 x E2 simultaneously to
// save on inversions.
// Returns 0 if any of Pi or Qi does not have order 2
// and 1 otherwise
int
action_by_translation_fp(translation_matrix_fp_t *Gi,
                      const theta_couple_point_fp_t *K1_4,
                      const theta_couple_point_fp_t *K2_4,
                      const theta_couple_curve_fp_t *E12)
{
    // Compute points of order 2 from Ki_4
    theta_couple_point_fp_t K1_2, K2_2;
    double_couple_point_fp(&K1_2, K1_4, E12);
    double_couple_point_fp(&K2_2, K2_4, E12);

    if (!verify_two_torsion_fp(&K1_2, &K2_2, E12)) {
        return 0;
    }

    // We need to invert four Z coordinates and
    // four determinants which we do with batched
    // inversion
    fp_t inverses[8];
    action_by_translation_z_and_det_fp(&inverses[0], &inverses[4], &K1_4->P1, &K1_2.P1);
    action_by_translation_z_and_det_fp(&inverses[1], &inverses[5], &K1_4->P2, &K1_2.P2);
    action_by_translation_z_and_det_fp(&inverses[2], &inverses[6], &K2_4->P1, &K2_2.P1);
    action_by_translation_z_and_det_fp(&inverses[3], &inverses[7], &K2_4->P2, &K2_2.P2);

    fp_batched_inv(inverses, 8);
    if (fp_is_zero(&inverses[0]))
        return 0; // something was wrong with our input (which somehow was not caught by
                  // verify_two_torsion_fp)

    action_by_translation_compute_matrix_fp(&Gi[0], &K1_4->P1, &K1_2.P1, &inverses[0], &inverses[4]);
    action_by_translation_compute_matrix_fp(&Gi[1], &K1_4->P2, &K1_2.P2, &inverses[1], &inverses[5]);
    action_by_translation_compute_matrix_fp(&Gi[2], &K2_4->P1, &K2_2.P1, &inverses[2], &inverses[6]);
    action_by_translation_compute_matrix_fp(&Gi[3], &K2_4->P2, &K2_2.P2, &inverses[3], &inverses[7]);

    return 1;
}

// Given the appropriate four torsion, computes the
// change of basis to compute the correct theta null
// point.
// Returns 0 if the order of K1_4 or K2_4 is not 4
int
gluing_change_of_basis_fp(basis_change_matrix_fp_t *M,
                       const theta_couple_point_fp_t *K1_4,
                       const theta_couple_point_fp_t *K2_4,
                       const theta_couple_curve_fp_t *E12)
{
    // Compute the four 2x2 matrices for the action by translation
    // on the four points:
    translation_matrix_fp_t Gi[4];
    if (!action_by_translation_fp(Gi, K1_4, K2_4, E12))
        return 0;

    // Computation of the 4x4 matrix from Mij
    // t001, t101 (resp t002, t102) first column of M11 * M21 (resp M12 * M22)
    fp_t t001, t101, t002, t102, tmp;

    fp_mul(&t001, &Gi[0].g00, &Gi[2].g00);
    fp_mul(&tmp, &Gi[0].g01, &Gi[2].g10);
    fp_add(&t001, &t001, &tmp);

    fp_mul(&t101, &Gi[0].g10, &Gi[2].g00);
    fp_mul(&tmp, &Gi[0].g11, &Gi[2].g10);
    fp_add(&t101, &t101, &tmp);

    fp_mul(&t002, &Gi[1].g00, &Gi[3].g00);
    fp_mul(&tmp, &Gi[1].g01, &Gi[3].g10);
    fp_add(&t002, &t002, &tmp);

    fp_mul(&t102, &Gi[1].g10, &Gi[3].g00);
    fp_mul(&tmp, &Gi[1].g11, &Gi[3].g10);
    fp_add(&t102, &t102, &tmp);

    // trace for the first row
    fp_set_one(&M->m[0][0]);
    fp_mul(&tmp, &t001, &t002);
    fp_add(&M->m[0][0], &M->m[0][0], &tmp);
    fp_mul(&tmp, &Gi[2].g00, &Gi[3].g00);
    fp_add(&M->m[0][0], &M->m[0][0], &tmp);
    fp_mul(&tmp, &Gi[0].g00, &Gi[1].g00);
    fp_add(&M->m[0][0], &M->m[0][0], &tmp);

    fp_mul(&M->m[0][1], &t001, &t102);
    fp_mul(&tmp, &Gi[2].g00, &Gi[3].g10);
    fp_add(&M->m[0][1], &M->m[0][1], &tmp);
    fp_mul(&tmp, &Gi[0].g00, &Gi[1].g10);
    fp_add(&M->m[0][1], &M->m[0][1], &tmp);

    fp_mul(&M->m[0][2], &t101, &t002);
    fp_mul(&tmp, &Gi[2].g10, &Gi[3].g00);
    fp_add(&M->m[0][2], &M->m[0][2], &tmp);
    fp_mul(&tmp, &Gi[0].g10, &Gi[1].g00);
    fp_add(&M->m[0][2], &M->m[0][2], &tmp);

    fp_mul(&M->m[0][3], &t101, &t102);
    fp_mul(&tmp, &Gi[2].g10, &Gi[3].g10);
    fp_add(&M->m[0][3], &M->m[0][3], &tmp);
    fp_mul(&tmp, &Gi[0].g10, &Gi[1].g10);
    fp_add(&M->m[0][3], &M->m[0][3], &tmp);

    // Compute the action of (0,out.K2_4.P2) for the second row
    fp_mul(&tmp, &Gi[3].g01, &M->m[0][1]);
    fp_mul(&M->m[1][0], &Gi[3].g00, &M->m[0][0]);
    fp_add(&M->m[1][0], &M->m[1][0], &tmp);

    fp_mul(&tmp, &Gi[3].g11, &M->m[0][1]);
    fp_mul(&M->m[1][1], &Gi[3].g10, &M->m[0][0]);
    fp_add(&M->m[1][1], &M->m[1][1], &tmp);

    fp_mul(&tmp, &Gi[3].g01, &M->m[0][3]);
    fp_mul(&M->m[1][2], &Gi[3].g00, &M->m[0][2]);
    fp_add(&M->m[1][2], &M->m[1][2], &tmp);

    fp_mul(&tmp, &Gi[3].g11, &M->m[0][3]);
    fp_mul(&M->m[1][3], &Gi[3].g10, &M->m[0][2]);
    fp_add(&M->m[1][3], &M->m[1][3], &tmp);

    // compute the action of (K1_4.P1,0) for the third row
    fp_mul(&tmp, &Gi[0].g01, &M->m[0][2]);
    fp_mul(&M->m[2][0], &Gi[0].g00, &M->m[0][0]);
    fp_add(&M->m[2][0], &M->m[2][0], &tmp);

    fp_mul(&tmp, &Gi[0].g01, &M->m[0][3]);
    fp_mul(&M->m[2][1], &Gi[0].g00, &M->m[0][1]);
    fp_add(&M->m[2][1], &M->m[2][1], &tmp);

    fp_mul(&tmp, &Gi[0].g11, &M->m[0][2]);
    fp_mul(&M->m[2][2], &Gi[0].g10, &M->m[0][0]);
    fp_add(&M->m[2][2], &M->m[2][2], &tmp);

    fp_mul(&tmp, &Gi[0].g11, &M->m[0][3]);
    fp_mul(&M->m[2][3], &Gi[0].g10, &M->m[0][1]);
    fp_add(&M->m[2][3], &M->m[2][3], &tmp);

    // compute the action of (K1_4.P1,K2_4.P2) for the final row
    fp_mul(&tmp, &Gi[0].g01, &M->m[1][2]);
    fp_mul(&M->m[3][0], &Gi[0].g00, &M->m[1][0]);
    fp_add(&M->m[3][0], &M->m[3][0], &tmp);

    fp_mul(&tmp, &Gi[0].g01, &M->m[1][3]);
    fp_mul(&M->m[3][1], &Gi[0].g00, &M->m[1][1]);
    fp_add(&M->m[3][1], &M->m[3][1], &tmp);

    fp_mul(&tmp, &Gi[0].g11, &M->m[1][2]);
    fp_mul(&M->m[3][2], &Gi[0].g10, &M->m[1][0]);
    fp_add(&M->m[3][2], &M->m[3][2], &tmp);

    fp_mul(&tmp, &Gi[0].g11, &M->m[1][3]);
    fp_mul(&M->m[3][3], &Gi[0].g10, &M->m[1][1]);
    fp_add(&M->m[3][3], &M->m[3][3], &tmp);

    return 1;
}

/**
 * @brief Compute the gluing isogeny from an elliptic product
 *
 * @param out Output: the theta_gluing_fp_t
 * @param K1_8 a couple point
 * @param E12 an elliptic curve product
 * @param K2_8 a point in E2[8]
 *
 * out : E1xE2 -> A of kernel [4](K1_8,K2_8)
 * if the kernel supplied has the incorrect order, or gluing seems malformed,
 * returns 0, otherwise returns 1.
 */
int
gluing_compute_fp(theta_gluing_fp_t *out,
               const theta_couple_curve_fp_t *E12,
               const theta_couple_jac_point_fp_t *xyK1_8,
               const theta_couple_jac_point_fp_t *xyK2_8,
               bool verify)
{
    // Ensure that we have been given the eight torsion
#ifndef NDEBUG
    {
        int check = point_jac_fp_test_order_twof(&xyK1_8->P1, &E12->E1, 3);
        if (!check)
            debug_print("xyK1_8->P1 does not have order 8");
        check = point_jac_fp_test_order_twof(&xyK2_8->P1, &E12->E1, 3);
        if (!check)
            debug_print("xyK2_8->P1 does not have order 8");
        check = point_jac_fp_test_order_twof(&xyK1_8->P2, &E12->E2, 3);
        if (!check)
            debug_print("xyK2_8->P1 does not have order 8");
        check = point_jac_fp_test_order_twof(&xyK2_8->P2, &E12->E2, 3);
        if (!check)
            debug_print("xyK2_8->P2 does not have order 8");
    }
#endif

    out->xyK1_8 = *xyK1_8;
    out->domain = *E12;

    // Given points in E[8] x E[8] we need the four torsion below
    theta_couple_jac_point_fp_t xyK1_4, xyK2_4;

    double_couple_jac_point_fp(&xyK1_4, xyK1_8, E12);
    double_couple_jac_point_fp(&xyK2_4, xyK2_8, E12);

    // Convert from (X:Y:Z) coordinates to (X:Z)
    theta_couple_point_fp_t K1_8, K2_8;
    theta_couple_point_fp_t K1_4, K2_4;

    couple_point_jac_to_ec_fp(&K1_8, xyK1_8);
    couple_point_jac_to_ec_fp(&K2_8, xyK2_8);
    couple_point_jac_to_ec_fp(&K1_4, &xyK1_4);
    couple_point_jac_to_ec_fp(&K2_4, &xyK2_4);

    // Set the basis change matrix, if we have not been given a valid K[8] for this computation
    // gluing_change_of_basis_fp will detect this and return 0
    if (!gluing_change_of_basis_fp(&out->M, &K1_4, &K2_4, E12)) {
        debug_print("gluing failed as kernel does not have correct order");
        return 0;
    }

    // apply the base change to the kernel
    theta_point_fp_t TT1, TT2;

    base_change_fp(&TT1, out, &K1_8);
    base_change_fp(&TT2, out, &K2_8);

    // compute the codomain
    to_squared_theta_fp(&TT1, &TT1);
    to_squared_theta_fp(&TT2, &TT2);

    // If the kernel is well formed then TT1.t and TT2.t are zero
    // if they are not, we exit early as the signature we are validating
    // is probably malformed
    if (!(fp_is_zero(&TT1.t) & fp_is_zero(&TT2.t))) {
        debug_print("gluing failed TT1.t or TT2.t is not zero");
        return 0;
    }
    // Test our projective factors are non zero
    if (fp_is_zero(&TT1.x) | fp_is_zero(&TT2.x) | fp_is_zero(&TT1.y) | fp_is_zero(&TT2.z) | fp_is_zero(&TT1.z))
        return 0; // invalid input

    // Projective factor: Ax
    fp_mul(&out->codomain.x, &TT1.x, &TT2.x);
    fp_mul(&out->codomain.y, &TT1.y, &TT2.x);
    fp_mul(&out->codomain.z, &TT1.x, &TT2.z);
    fp_set_zero(&out->codomain.t);
    // Projective factor: ABCxz
    fp_mul(&out->precomputation.x, &TT1.y, &TT2.z);
    fp_copy(&out->precomputation.y, &out->codomain.z);
    fp_copy(&out->precomputation.z, &out->codomain.y);
    fp_set_zero(&out->precomputation.t);

    // Compute the two components of phi(K1_8) = (x:x:y:y).
    fp_mul(&out->imageK1_8.x, &TT1.x, &out->precomputation.x);
    fp_mul(&out->imageK1_8.y, &TT1.z, &out->precomputation.z);

    // If K1_8 and K2_8 are our 8-torsion points, this ensures that the
    // 4-torsion points [2]K1_8 and [2]K2_8 are isotropic.
    if (verify) {
        fp_t t1, t2;
        fp_mul(&t1, &TT1.y, &out->precomputation.y);
        if (!fp_is_equal(&out->imageK1_8.x, &t1))
            return 0;
        fp_mul(&t1, &TT2.x, &out->precomputation.x);
        fp_mul(&t2, &TT2.z, &out->precomputation.z);
        if (!fp_is_equal(&t2, &t1))
            return 0;
    }

    // compute the final codomain
    hadamard_fp(&out->codomain, &out->codomain);
    return 1;
}

// sub routine of the gluing eval
void
gluing_eval_point_fp(theta_point_fp_t *image, const theta_couple_jac_point_fp_t *P, const theta_gluing_fp_t *phi)
{
    // #ifndef NDEBUG
    //     uint32_t fp_mul_start = N_FP_MUL;
    //     uint32_t fp_sqr_start = N_FP_SQR;
    // #endif

    theta_point_fp_t T1, T2;
    add_components_fp_t add_comp1, add_comp2;

    // Compute the cross addition components of P1+Q1 and P2+Q2
    point_jac_fp_to_xz_add_components(&add_comp1, &P->P1, &phi->xyK1_8.P1, &phi->domain.E1);
    point_jac_fp_to_xz_add_components(&add_comp2, &P->P2, &phi->xyK1_8.P2, &phi->domain.E2);

    // Compute T1 and T2 derived from the cross addition components.
    fp_mul(&T1.x, &add_comp1.u, &add_comp2.u); // T1x = u1u2
    fp_mul(&T2.t, &add_comp1.v, &add_comp2.v); // T2t = v1v2
    fp_add(&T1.x, &T1.x, &T2.t);               // T1x = u1u2 + v1v2
    fp_mul(&T1.y, &add_comp1.u, &add_comp2.w); // T1y = u1w2
    fp_mul(&T1.z, &add_comp1.w, &add_comp2.u); // T1z = w1u2
    fp_mul(&T1.t, &add_comp1.w, &add_comp2.w); // T1t = w1w2
    fp_add(&T2.x, &add_comp1.u, &add_comp1.v); // T2x = (u1+v1)
    fp_add(&T2.y, &add_comp2.u, &add_comp2.v); // T2y = (u2+v2)
    fp_mul(&T2.x, &T2.x, &T2.y);               // T2x = (u1+v1)(u2+v2)
    fp_sub(&T2.x, &T2.x, &T1.x);               // T1x = v1u2 + u1v2
    fp_mul(&T2.y, &add_comp1.v, &add_comp2.w); // T2y = v1w2
    fp_mul(&T2.z, &add_comp1.w, &add_comp2.v); // T2z = w1v2
    fp_set_zero(&T2.t);                        // T2t = 0

    // Apply the basis change and compute their respective square
    // theta(P+Q) = M.T1 - M.T2 and theta(P-Q) = M.T1 + M.T2
    apply_isomorphism_general_fp(&T1, &phi->M, &T1, true);
    apply_isomorphism_general_fp(&T2, &phi->M, &T2, false);
    pointwise_square_fp(&T1, &T1);
    pointwise_square_fp(&T2, &T2);

    // the difference between the two is therefore theta(P+Q)theta(P-Q)
    // whose hadamard transform is then the product of the dual
    // theta_point_fp_ts of phi(P) and phi(Q).
    fp_sub(&T1.x, &T1.x, &T2.x);
    fp_sub(&T1.y, &T1.y, &T2.y);
    fp_sub(&T1.z, &T1.z, &T2.z);
    fp_sub(&T1.t, &T1.t, &T2.t);
    hadamard_fp(&T1, &T1);

    // Compute (x, y, z, t)
    // As imageK1_8 = (x:x:y:y), its inverse is (y:y:x:x).
    fp_mul(&image->x, &T1.x, &phi->imageK1_8.y);
    fp_mul(&image->y, &T1.y, &phi->imageK1_8.y);
    fp_mul(&image->z, &T1.z, &phi->imageK1_8.x);
    fp_mul(&image->t, &T1.t, &phi->imageK1_8.x);

    hadamard_fp(image, image);

    // #ifndef NDEBUG
    //     printf("Took %d fp multiplications, ", N_FP_MUL - fp_mul_start);
    //     printf("Took %d fp squarings\n", N_FP_SQR - fp_sqr_start);
    // #endif
}

/**
 * @brief Evaluate a gluing isogeny from an elliptic product on a basis
 *
 * @param image1 Output: the theta_point_fp_t of the image of the first couple of points
 * @param image2 Output : the theta point of the image of the second couple of points
 * @param xyT1: A pair of points (X : Y : Z) on E1E2 to glue using phi
 * @param xyT2: A pair of points (X : Y : Z) on E1E2 to glue using phi
 * @param phi : a gluing isogeny E1 x E2 -> A
 *
 **/
void
gluing_eval_basis_fp(theta_point_fp_t *image1,
                  theta_point_fp_t *image2,
                  const theta_couple_jac_point_fp_t *xyT1,
                  const theta_couple_jac_point_fp_t *xyT2,
                  const theta_gluing_fp_t *phi)
{
    gluing_eval_point_fp(image1, xyT1, phi);
    gluing_eval_point_fp(image2, xyT2, phi);
}

/**
 * @brief Compute a (2,2) isogeny in dimension 2 in the theta_model
 *
 * @param out Output: the theta_isogeny_fp
 * @param A a theta null point for the domain
 * @param T1_8 a point in A[8]
 * @param T2_8 a point in A[8]
 * @param hadamard_bool_1 a boolean used for the last two steps of the chain
 * @param hadamard_bool_2 a boolean used for the last two steps of the chain
 *
 * out : A -> B of kernel [4](T1_8,T2_8)
 * hadamard_bool_1 controls if the domain is in standard or dual coordinates
 * hadamard_bool_2 controls if the codomain is in standard or dual coordinates
 * verify: add extra sanity check to ensure our 8-torsion points are coherent with the isogeny
 *
 */
int
theta_isogeny_fp_compute(theta_isogeny_fp_t *out,
                      const theta_structure_fp_t *A,
                      const theta_point_fp_t *T1_8,
                      const theta_point_fp_t *T2_8,
                      bool hadamard_bool_1,
                      bool hadamard_bool_2,
                      bool verify)
{
    out->hadamard_bool_1 = hadamard_bool_1;
    out->hadamard_bool_2 = hadamard_bool_2;
    out->domain = *A;
    out->T1_8 = *T1_8;
    out->T2_8 = *T2_8;
    out->codomain.precomputation = false;

    theta_point_fp_t TT1, TT2;

    if (hadamard_bool_1) {
        hadamard_fp(&TT1, T1_8);
        to_squared_theta_fp(&TT1, &TT1);
        hadamard_fp(&TT2, T2_8);
        to_squared_theta_fp(&TT2, &TT2);
    } else {
        to_squared_theta_fp(&TT1, T1_8);
        to_squared_theta_fp(&TT2, T2_8);
    }

    fp_t t1, t2;

    // Test that our projective factor ABCDxzw is non zero, where
    // TT1=(Ax, Bx, Cy, Dy), TT2=(Az, Bw, Cz, Dw)
    // But ABCDxzw=0 can only happen if we had an unexpected splitting in
    // the isogeny chain.
    // In either case reject
    // (this is not strictly necessary, we could just return (0:0:0:0))
    if (fp_is_zero(&TT2.x)
        || fp_is_zero(&TT2.y)
        || fp_is_zero(&TT2.z)
        || fp_is_zero(&TT2.t)
        || fp_is_zero(&TT1.x)
        || fp_is_zero(&TT1.y)
    ) {
        debug_print(":: theta_isogeny_fp_compute: Early splitting");
        return 0;
    }

    fp_mul(&t1, &TT1.x, &TT2.y);
    fp_mul(&t2, &TT1.y, &TT2.x);
    fp_mul(&out->codomain.null_point.x, &TT2.x, &t1);
    fp_mul(&out->codomain.null_point.y, &TT2.y, &t2);
    fp_mul(&out->codomain.null_point.z, &TT2.z, &t1);
    fp_mul(&out->codomain.null_point.t, &TT2.t, &t2);
    fp_t t3;
    fp_mul(&t3, &TT2.z, &TT2.t);
    fp_mul(&out->precomputation.x, &t3, &TT1.y);
    fp_mul(&out->precomputation.y, &t3, &TT1.x);
    fp_copy(&out->precomputation.z, &out->codomain.null_point.t);
    fp_copy(&out->precomputation.t, &out->codomain.null_point.z);

    // If T1_8 and T2_8 are our 8-torsion points, this ensures that the
    // 4-torsion points 2T1_8 and 2T2_8 are isotropic.
    if (verify) {
        fp_mul(&t1, &TT1.x, &out->precomputation.x);
        fp_mul(&t2, &TT1.y, &out->precomputation.y);
        if (!fp_is_equal(&t1, &t2)) {
            debug_print(":: theta_isogeny_fp_compute: Precompute 1 failed");
            fp_print("t1 ", &t1);
            fp_print("t2 ", &t2);
            return 0;
        }
        fp_mul(&t1, &TT1.z, &out->precomputation.z);
        fp_mul(&t2, &TT1.t, &out->precomputation.t);
        if (!fp_is_equal(&t1, &t2)) {
            debug_print(":: theta_isogeny_fp_compute: Precompute 2 failed");
            fp_print("t1 ", &t1);
            fp_print("t2 ", &t2);
            return 0;
        }
        fp_mul(&t1, &TT2.x, &out->precomputation.x);
        fp_mul(&t2, &TT2.z, &out->precomputation.z);
        if (!fp_is_equal(&t1, &t2)) {
            debug_print(":: theta_isogeny_fp_compute: Precompute 3 failed");
            fp_print("t1 ", &t1);
            fp_print("t2 ", &t2);
            return 0;
        }
        fp_mul(&t1, &TT2.y, &out->precomputation.y);
        fp_mul(&t2, &TT2.t, &out->precomputation.t);
        if (!fp_is_equal(&t1, &t2)) {
            debug_print(":: theta_isogeny_fp_compute: Precompute 4 failed");
            fp_print("t1 ", &t1);
            fp_print("t2 ", &t2);
            return 0;
        }
    }

    if (hadamard_bool_2) {
        hadamard_fp(&out->codomain.null_point, &out->codomain.null_point);
    }
    return 1;
}

void
theta_isogeny_fp_eval(theta_point_fp_t *out, const theta_isogeny_fp_t *phi, const theta_point_fp_t *P)
{
    if (phi->hadamard_bool_1) {
        hadamard_fp(out, P);
        to_squared_theta_fp(out, out);
    } else {
        to_squared_theta_fp(out, P);
    }
    fp_mul(&out->x, &out->x, &phi->precomputation.x);
    fp_mul(&out->y, &out->y, &phi->precomputation.y);
    fp_mul(&out->z, &out->z, &phi->precomputation.z);
    fp_mul(&out->t, &out->t, &phi->precomputation.t);

    if (phi->hadamard_bool_2) {
        hadamard_fp(out, out);
    }
}

// bool
// splitting_compute(theta_splitting_fp_t *out, const theta_structure_fp_t *A, int zero_index)
// {
//     // init
//     uint32_t ctl;
//     uint32_t count = 0;
//     fp_t U_cst, t1, t2;
//
//     memset(&out->M, 0, sizeof(basis_change_matrix_fp_t));
//
//     // enumerate through all indices
//     for (int i = 0; i < 10; i++) {
//         fp_set_zero(&U_cst);
//         for (int t = 0; t < 4; t++) {
//             // Iterate through the null point
//             choose_index_theta_point_fp(&t2, t, &A->null_point);
//             choose_index_theta_point_fp(&t1, t ^ EVEN_INDEX[i][1], &A->null_point);
//
//             // Compute t1 * t2
//             fp_mul(&t1, &t1, &t2);
//             // If CHI_EVAL(i,t) is +1 we want ctl to be 0 and
//             // If CHI_EVAL(i,t) is -1 we want ctl to be 0xFF..FF
//             ctl = (uint32_t)(CHI_EVAL[EVEN_INDEX[i][0]][t] >> 1);
//             assert(ctl == 0 || ctl == 0xffffffff);
//
//             fp_neg(&t2, &t1);
//             fp_select(&t1, &t1, &t2, ctl);
//
//             // Then we compute U_cst ± (t1 * t2)
//             fp_add(&U_cst, &U_cst, &t1);
//         }
//
//         // If U_cst is 0 then update the splitting matrix
//         ctl = fp_is_zero(&U_cst);
//         count -= ctl;
//         select_base_change_matrix(&out->M, &out->M, &SPLITTING_TRANSFORMS[i], ctl);
//         if (zero_index != -1 && i == zero_index &&
//             !ctl) { // extra checks if we know exactly where the 0 index should be
//             return 0;
//         }
//     }
//
//     // apply the isomorphism to ensure the null point is compatible with splitting
//     apply_isomorphism_fp(&out->B.null_point, &out->M, &A->null_point);
//
//     // splitting was successful only if exactly one zero was identified
//     return count == 1;
// }

// int
// theta_product_structure_to_elliptic_product(theta_couple_curve_fp_t *E12, theta_structure_fp_t *A)
// {
//     fp_t xx, yy;
//
//     // This should be true from our computations in splitting_compute
//     // but still check this for sanity
//     if (!is_product_theta_point_fp_t(&A->null_point))
//         return 0;
//
//     curve_mg_fp_init(&(E12->E1));
//     curve_mg_fp_init(&(E12->E2));
//
//     // A valid elliptic theta null point has no zero coordinate
//     if (fp_is_zero(&A->null_point.x) | fp_is_zero(&A->null_point.y) | fp_is_zero(&A->null_point.z))
//         return 0;
//
//     // xx = x², yy = y²
//     fp_sqr(&xx, &A->null_point.x);
//     fp_sqr(&yy, &A->null_point.y);
//     // xx = x^4, yy = y^4
//     fp_sqr(&xx, &xx);
//     fp_sqr(&yy, &yy);
//
//     // A2 = -2(x^4+y^4)/(x^4-y^4)
//     fp_add(&E12->E2.A, &xx, &yy);
//     fp_sub(&E12->E2.C, &xx, &yy);
//     fp_add(&E12->E2.A, &E12->E2.A, &E12->E2.A);
//     fp_neg(&E12->E2.A, &E12->E2.A);
//
//     // same with x,z
//     fp_sqr(&xx, &A->null_point.x);
//     fp_sqr(&yy, &A->null_point.z);
//     fp_sqr(&xx, &xx);
//     fp_sqr(&yy, &yy);
//
//     // A1 = -2(x^4+z^4)/(x^4-z^4)
//     fp_add(&E12->E1.A, &xx, &yy);
//     fp_sub(&E12->E1.C, &xx, &yy);
//     fp_add(&E12->E1.A, &E12->E1.A, &E12->E1.A);
//     fp_neg(&E12->E1.A, &E12->E1.A);
//
//     if (fp_is_zero(&E12->E1.C) | fp_is_zero(&E12->E2.C))
//         return 0;
//
//     return 1;
// }

int
theta_point_fp_to_montgomery_point(theta_couple_point_fp_t *P12, const theta_point_fp_t *P, const theta_structure_fp_t *A)
{
    fp_t temp;
    const fp_t *x, *z;

    if (!is_product_theta_point_fp_t(P))
        return 0;

    x = &P->x;
    z = &P->y;
    if (fp_is_zero(x) & fp_is_zero(z)) {
        x = &P->z;
        z = &P->t;
    }
    if (fp_is_zero(x) & fp_is_zero(z)) {
        return 0; // at this point P=(0:0:0:0) so is invalid
    }
    // P2.X = A.null_point.y * P.x + A.null_point.x * P.y
    // P2.Z = - A.null_point.y * P.x + A.null_point.x * P.y
    fp_mul(&P12->P2.x, &A->null_point.y, x);
    fp_mul(&temp, &A->null_point.x, z);
    fp_sub(&P12->P2.z, &temp, &P12->P2.x);
    fp_add(&P12->P2.x, &P12->P2.x, &temp);

    x = &P->x;
    z = &P->z;
    if (fp_is_zero(x) & fp_is_zero(z)) {
        x = &P->y;
        z = &P->t;
    }
    // P1.X = A.null_point.z * P.x + A.null_point.x * P.z
    // P1.Z = -A.null_point.z * P.x + A.null_point.x * P.z
    fp_mul(&P12->P1.x, &A->null_point.z, x);
    fp_mul(&temp, &A->null_point.x, z);
    fp_sub(&P12->P1.z, &temp, &P12->P1.x);
    fp_add(&P12->P1.x, &P12->P1.x, &temp);
    return 1;
}

// uint32_t
// _theta_chain_compute_impl_fp(
//     theta_couple_curve_fp_t *E34,
//     theta_couple_curve_fp_t *E12,
//     const uint32_t chain_length,
//     theta_couple_jac_point_fp_t *C1_jac_dbls,
//     theta_couple_jac_point_fp_t *C2_jac_dbls,
//     uint16_t *strategy,
//     const uint16_t strategy_length,
//     const bool verify
// ) {
//     theta_structure_fp_t theta;
//
//     clock_t gluing_start = clock();
//
//     // kernel points for the remaining isogeny steps
//     theta_point_fp_t thetaQ1[MAX_STRATEGY_LENGTH], thetaQ2[MAX_STRATEGY_LENGTH];
//
//     int current = strategy_length - 1;
//     // the gluing step
//     theta_gluing_fp_t first_step;
//     {
//         assert(strategy[current] == 1);
//
//         // compute the gluing isogeny
//         if (!gluing_compute_fp(&first_step, E12, &C1_jac_dbls[current], &C2_jac_dbls[current], verify)) {
//             debug_print("Computing the gluing failed");
//             return 0;
//         }
//
//         // push kernel points through gluing isogeny
//         for (int j = 0; j < current; ++j) {
//             gluing_eval_basis_fp(&thetaQ1[j], &thetaQ2[j], &C1_jac_dbls[j], &C2_jac_dbls[j], &first_step);
//             --strategy[j];
//         }
//
//         --current;
//     }
//
//     printf(":: [Gluing] Total took %.1Lf us\n", (long double)((clock() - gluing_start) * 1000000 / CLOCKS_PER_SEC));
//
//     // set-up the theta_structure_fp_t for the first codomain
//     theta.null_point = first_step.codomain;
//     theta.precomputation = 0;
//     theta_precomputation_fp(&theta);
//
//     theta_isogeny_fp_t step;
//
//     // and now we do the remaining steps
//     for (unsigned i = 1; current >= 0 && strategy[current]; ++i) {
//         assert(current < MAX_STRATEGY_LENGTH);
//         while (strategy[current] != 1) {
//             assert(strategy[current] >= 2);
//             ++current;
//             assert(current < MAX_STRATEGY_LENGTH);
//             const unsigned num_dbls = strategy[current - 1] / 2;
//             assert(num_dbls && num_dbls < strategy[current - 1]);
//             double_iter_fp(&thetaQ1[current], &theta, &thetaQ1[current - 1], num_dbls);
//             double_iter_fp(&thetaQ2[current], &theta, &thetaQ2[current - 1], num_dbls);
//             strategy[current] = strategy[current - 1] - num_dbls;
//         }
//
//         // computing the next step
//         int ret;
//         if (i == chain_length - 2)
//             ret = theta_isogeny_fp_compute(&step, &theta, &thetaQ1[current], &thetaQ2[current], 0, 0, verify);
//         else if (i == chain_length - 1)
//             ret = theta_isogeny_fp_compute(&step, &theta, &thetaQ1[current], &thetaQ2[current], 1, 0, false);
//         else
//             ret = theta_isogeny_fp_compute(&step, &theta, &thetaQ1[current], &thetaQ2[current], 0, 1, verify);
//
//         if (!ret) {
//             debug_print("Theta isogeny chain failed somewhere after gluing");
//             return 0;
//         }
//
//         // updating the codomain
//         theta = step.codomain;
//
//         // pushing the kernel
//         assert(strategy[current] == 1);
//         for (int j = 0; j < current; ++j) {
//             theta_isogeny_fp_eval(&thetaQ1[j], &step, &thetaQ1[j]);
//             theta_isogeny_fp_eval(&thetaQ2[j], &step, &thetaQ2[j]);
//             assert(strategy[j]);
//             --strategy[j];
//         }
//
//         --current;
//     }
//
//     assert(current == -1);
//
//     // final splitting step
//     theta_splitting_fp_t last_step;
//
//     bool is_split = splitting_compute(&last_step, &theta, 8);
//
//     if (!is_split) {
//         debug_print("kernel did not generate an isogeny between elliptic products");
//         return 0;
//     }
//
//     if (!theta_product_structure_to_elliptic_product(E34, &last_step.B))
//         return 0;
//
//     return 1;
// }

// uint32_t
// theta_chain_compute_fp(
//     theta_couple_curve_fp_t *E34,
//     theta_couple_curve_fp_t *E12,
//     const uint32_t chain_length,
//     theta_couple_jac_point_fp_t *C1_jac_dbls,
//     theta_couple_jac_point_fp_t *C2_jac_dbls,
//     uint16_t *strategy,
//     const uint16_t strategy_length
// ) {
//     printf("HD chain length: %d\n", chain_length);
//     return _theta_chain_compute_impl_fp(E34, E12, chain_length, C1_jac_dbls, C2_jac_dbls, strategy, strategy_length, false);
// }
