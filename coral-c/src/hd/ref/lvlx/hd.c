#include <hd.h>
#include <assert.h>

void
double_couple_point_fp2(theta_couple_point_fp2_t *out, const theta_couple_point_fp2_t *in, const theta_couple_curve_fp2_t *E1E2)
{
    xpoint_eqw_fp2_dbl(&out->P1, &in->P1, &E1E2->E1);
    xpoint_eqw_fp2_dbl(&out->P2, &in->P2, &E1E2->E2);
}

void
double_couple_jac_point_fp2(theta_couple_jac_point_fp2_t *out,
                        const theta_couple_jac_point_fp2_t *in,
                        const theta_couple_curve_fp2_t *E1E2)
{
    point_jac_fp2_dbl(&out->P1, &in->P1, &E1E2->E1);
    point_jac_fp2_dbl(&out->P2, &in->P2, &E1E2->E2);
}

void
couple_point_jac_to_ec_fp2(theta_couple_point_fp2_t *P, const theta_couple_jac_point_fp2_t *xyP)
{
    point_jac_fp2_to_ec(&P->P1, &xyP->P1);
    point_jac_fp2_to_ec(&P->P2, &xyP->P2);
}

void
double_couple_jac_point_iter_fp2(theta_couple_jac_point_fp2_t *out,
                             unsigned n,
                             const theta_couple_jac_point_fp2_t *in,
                             const theta_couple_curve_fp2_t *E1E2)
{
    if (n == 0) {
        *out = *in;
    } else if (n == 1) {
        double_couple_jac_point_fp2(out, in, E1E2);
    } else {
        fp2_t a1, a2, t1, t2;

        jac_to_ws_fp2(&out->P1, &t1, &a1, &in->P1, &E1E2->E1);
        jac_to_ws_fp2(&out->P2, &t2, &a2, &in->P2, &E1E2->E2);

        point_jac_fp2_dblw(&out->P1, &t1, &out->P1, &t1);
        point_jac_fp2_dblw(&out->P2, &t2, &out->P2, &t2);
        for (unsigned i = 0; i < n - 1; i++) {
            point_jac_fp2_dblw(&out->P1, &t1, &out->P1, &t1);
            point_jac_fp2_dblw(&out->P2, &t2, &out->P2, &t2);
        }

        jac_from_ws_fp2(&out->P1, &out->P1, &a1, &E1E2->E1);
        jac_from_ws_fp2(&out->P2, &out->P2, &a2, &E1E2->E2);
    }
}
