#include <assert.h>

#include "theta_structure.h"

void
theta_precomputation_fp(theta_structure_fp_t *A)
{

    if (A->precomputation) {
        return;
    }

    theta_point_fp_t A_dual;
    to_squared_theta_fp(&A_dual, &A->null_point);

    fp_t t1, t2;
    fp_mul(&t1, &A_dual.x, &A_dual.y);
    fp_mul(&t2, &A_dual.z, &A_dual.t);
    fp_mul(&A->XYZ0, &t1, &A_dual.z);
    fp_mul(&A->XYT0, &t1, &A_dual.t);
    fp_mul(&A->YZT0, &t2, &A_dual.y);
    fp_mul(&A->XZT0, &t2, &A_dual.x);

    fp_mul(&t1, &A->null_point.x, &A->null_point.y);
    fp_mul(&t2, &A->null_point.z, &A->null_point.t);
    fp_mul(&A->xyz0, &t1, &A->null_point.z);
    fp_mul(&A->xyt0, &t1, &A->null_point.t);
    fp_mul(&A->yzt0, &t2, &A->null_point.y);
    fp_mul(&A->xzt0, &t2, &A->null_point.x);

    A->precomputation = true;
}

void
double_point_fp(theta_point_fp_t *out, theta_structure_fp_t *A, const theta_point_fp_t *in)
{
    to_squared_theta_fp(out, in);
    fp_sqr(&out->x, &out->x);
    fp_sqr(&out->y, &out->y);
    fp_sqr(&out->z, &out->z);
    fp_sqr(&out->t, &out->t);

    if (!A->precomputation) {
        theta_precomputation_fp(A);
    }
    fp_mul(&out->x, &out->x, &A->YZT0);
    fp_mul(&out->y, &out->y, &A->XZT0);
    fp_mul(&out->z, &out->z, &A->XYT0);
    fp_mul(&out->t, &out->t, &A->XYZ0);

    hadamard_fp(out, out);

    fp_mul(&out->x, &out->x, &A->yzt0);
    fp_mul(&out->y, &out->y, &A->xzt0);
    fp_mul(&out->z, &out->z, &A->xyt0);
    fp_mul(&out->t, &out->t, &A->xyz0);
}

void
double_iter_fp(theta_point_fp_t *out, theta_structure_fp_t *A, const theta_point_fp_t *in, int exp)
{
    if (exp == 0) {
        *out = *in;
    } else {
        double_point_fp(out, A, in);
        for (int i = 1; i < exp; i++) {
            double_point_fp(out, A, out);
        }
    }
}

uint32_t
is_product_theta_point_fp_t(const theta_point_fp_t *P)
{
    fp_t t1, t2;
    fp_mul(&t1, &P->x, &P->t);
    fp_mul(&t2, &P->y, &P->z);
    return fp_is_equal(&t1, &t2);
}
