/** @file
 *
 * @authors Antonin Leroux
 *
 * @brief the theta structure header
 */

#ifndef THETA_STRUCTURE_H
#define THETA_STRUCTURE_H

#include "ec.h"
#include "fp2.h"
#include "hd.h"

// *** Fp2 ***
// (x, y, z, t) =  (x + y + z + t, x - y + z - t, x + y - z - t, x - y - z + t)
static inline void
hadamard_fp2(theta_point_fp2_t *out, const theta_point_fp2_t *in)
{
    fp2_t t1, t2, t3, t4;

    fp2_add(&t1, &in->x, &in->y);
    fp2_sub(&t2, &in->x, &in->y);
    fp2_add(&t3, &in->z, &in->t);
    fp2_sub(&t4, &in->z, &in->t);

    fp2_add(&out->x, &t1, &t3);
    fp2_add(&out->y, &t2, &t4);
    fp2_sub(&out->z, &t1, &t3);
    fp2_sub(&out->t, &t2, &t4);
}

static inline void
pointwise_square_fp2(theta_point_fp2_t *out, const theta_point_fp2_t *in)
{
    fp2_sqr(&out->x, &in->x);
    fp2_sqr(&out->y, &in->y);
    fp2_sqr(&out->z, &in->z);
    fp2_sqr(&out->t, &in->t);
}

static inline void
to_squared_theta_fp2(theta_point_fp2_t *out, const theta_point_fp2_t *in)
{
    pointwise_square_fp2(out, in);
    hadamard_fp2(out, out);
}

/**
 * @brief Perform the theta structure precomputation
 *
 * @param A Output: the theta_structure_fp2_t
 *
 * if A.null_point = (x,y,z,t)
 * if (xx,yy,zz,tt) = to_squared_theta_fp2(A.null_point)
 * Computes y0,z0,t0,Y0,Z0,T0 = x/y,x/z,x/t,XX/YY,XX/ZZ,XX/TT
 *
 */
void theta_precomputation_fp2(theta_structure_fp2_t *A);

// Note: assumes that no coordinates are zero and that the precomputation of A has been done
void double_point_fp2(theta_point_fp2_t *out, theta_structure_fp2_t *A, const theta_point_fp2_t *in);
void double_iter_fp2(theta_point_fp2_t *out, theta_structure_fp2_t *A, const theta_point_fp2_t *in, int exp);

// returns 0xFFFFFFFF if true, zero otherwise
uint32_t is_product_theta_point_fp2_t(const theta_point_fp2_t *P);

// *** Fp ***
// (x, y, z, t) =  (x + y + z + t, x - y + z - t, x + y - z - t, x - y - z + t)
static inline void
hadamard_fp(theta_point_fp_t *out, const theta_point_fp_t *in)
{
    fp_t t1, t2, t3, t4;

    fp_add(&t1, &in->x, &in->y);
    fp_sub(&t2, &in->x, &in->y);
    fp_add(&t3, &in->z, &in->t);
    fp_sub(&t4, &in->z, &in->t);

    fp_add(&out->x, &t1, &t3);
    fp_add(&out->y, &t2, &t4);
    fp_sub(&out->z, &t1, &t3);
    fp_sub(&out->t, &t2, &t4);
}

static inline void
pointwise_square_fp(theta_point_fp_t *out, const theta_point_fp_t *in)
{
    fp_sqr(&out->x, &in->x);
    fp_sqr(&out->y, &in->y);
    fp_sqr(&out->z, &in->z);
    fp_sqr(&out->t, &in->t);
}

static inline void
to_squared_theta_fp(theta_point_fp_t *out, const theta_point_fp_t *in)
{
    pointwise_square_fp(out, in);
    hadamard_fp(out, out);
}

/**
 * @brief Perform the theta structure precomputation
 *
 * @param A Output: the theta_structure_fp_t
 *
 * if A.null_point = (x,y,z,t)
 * if (xx,yy,zz,tt) = to_squared_theta_fp(A.null_point)
 * Computes y0,z0,t0,Y0,Z0,T0 = x/y,x/z,x/t,XX/YY,XX/ZZ,XX/TT
 *
 */
void theta_precomputation_fp(theta_structure_fp_t *A);

// Note: assumes that no coordinates are zero and that the precomputation of A has been done
void double_point_fp(theta_point_fp_t *out, theta_structure_fp_t *A, const theta_point_fp_t *in);
void double_iter_fp(theta_point_fp_t *out, theta_structure_fp_t *A, const theta_point_fp_t *in, int exp);

// returns 0xFFFFFFFF if true, zero otherwise
uint32_t is_product_theta_point_fp_t(const theta_point_fp_t *P);

#endif
