#include <hd.h>
#include <assert.h>

void
double_couple_point_fp(theta_couple_point_fp_t *out, const theta_couple_point_fp_t *in, const theta_couple_curve_fp_t *E1E2)
{
    xpoint_eqw_fp_dbl(&out->P1, &in->P1, &E1E2->E1);
    xpoint_eqw_fp_dbl(&out->P2, &in->P2, &E1E2->E2);
}

void
double_couple_jac_point_fp(theta_couple_jac_point_fp_t *out,
                        const theta_couple_jac_point_fp_t *in,
                        const theta_couple_curve_fp_t *E1E2)
{
    point_jac_fp_dbl(&out->P1, &in->P1, &E1E2->E1);
    point_jac_fp_dbl(&out->P2, &in->P2, &E1E2->E2);
}

void
couple_point_jac_to_ec_fp(theta_couple_point_fp_t *P, const theta_couple_jac_point_fp_t *xyP)
{
    point_jac_fp_to_ec(&P->P1, &xyP->P1);
    point_jac_fp_to_ec(&P->P2, &xyP->P2);
}
