#ifndef EC_H
#define EC_H
#include <assert.h>
#include <stdio.h>

#include "sqisign_namespace.h"

#include "ec_params.h"
#include "fp2.h"
#include "tools.h"

// --------------------------------------------------------------------------------
// Points and Bases
// --------------------------------------------------------------------------------

// eqw = Equally weighted (X: Y: Z) represents (X / Z, Y / Z)
// jac = Jacobian weighted (X: Y: Z) represents (X / Z^2, Y / Z^3)

// *** Fp2 ***
typedef struct point_eqw_fp2_t  { fp2_t x; fp2_t y; fp2_t z; } point_eqw_fp2_t;
void point_eqw_fp2_init(point_eqw_fp2_t *P);
void point_eqw_fp2_normalise(point_eqw_fp2_t *P);
void point_eqw_fp2_copy(point_eqw_fp2_t *P, const point_eqw_fp2_t *Q);

typedef struct xpoint_eqw_fp2_t { fp2_t x; fp2_t z; } xpoint_eqw_fp2_t;
void xpoint_eqw_fp2_init(xpoint_eqw_fp2_t *P);
void xpoint_eqw_fp2_normalise(xpoint_eqw_fp2_t *P);
void xpoint_eqw_fp2_copy(xpoint_eqw_fp2_t *P, const xpoint_eqw_fp2_t *Q);

typedef struct point_jac_fp2_t { fp2_t x; fp2_t y; fp2_t z; } point_jac_fp2_t;
void point_jac_fp2_init(point_jac_fp2_t *P);
void point_jac_fp2_copy(point_jac_fp2_t *P, const point_jac_fp2_t *Q);
void point_jac_fp2_normalise(point_jac_fp2_t *P);

typedef struct add_components_fp2_t { fp2_t u; fp2_t v; fp2_t w; } add_components_fp2_t;

// *** Fp ***
typedef struct point_eqw_fp_t  { fp_t x; fp_t y; fp_t z; } point_eqw_fp_t;
void point_eqw_fp_init(point_eqw_fp_t *P);
void point_eqw_fp_normalise(point_eqw_fp_t *P);
void point_eqw_fp_copy(point_eqw_fp_t *P, const point_eqw_fp_t *Q);

typedef struct xpoint_eqw_fp_t { fp_t x; fp_t z; } xpoint_eqw_fp_t;
void xpoint_eqw_fp_init(xpoint_eqw_fp_t *P);
void xpoint_eqw_fp_normalise(xpoint_eqw_fp_t *P);
void xpoint_eqw_fp_copy(xpoint_eqw_fp_t *P, const xpoint_eqw_fp_t *Q);

typedef struct point_jac_fp_t { fp_t x; fp_t y; fp_t z; } point_jac_fp_t;
void point_jac_fp_init(point_jac_fp_t *P);
void point_jac_fp_copy(point_jac_fp_t *P, const point_jac_fp_t *Q);
void point_jac_fp_normalise(point_jac_fp_t *P);

typedef struct add_components_fp_t { fp_t u; fp_t v; fp_t w; } add_components_fp_t;

// --------------------------------------------------------------------------------
// Curves
// --------------------------------------------------------------------------------
// mg = Montgomery
// sw = Short Weierstrass
// A24 is the point (A+2 : 4C)

// *** Fp2 ***
typedef struct curve_mg_fp2_t { fp2_t A; fp2_t C; xpoint_eqw_fp2_t A24; bool is_A24_computed_and_normalized; } curve_mg_fp2_t;
typedef struct curve_sw_fp2_t { fp2_t A; fp2_t B; fp2_t C; } curve_sw_fp2_t;

void curve_mg_fp2_init(curve_mg_fp2_t *E);
void xpoint_eqw_fp2_init(xpoint_eqw_fp2_t *P);

int curve_mg_fp2_verify_A(const fp2_t *A);
int curve_mg_fp2_init_from_A(curve_mg_fp2_t *E, const fp2_t *A);

void curve_mg_fp2_normalise(curve_mg_fp2_t *E);
void curve_mg_fp2_normalise_A24(curve_mg_fp2_t *E);
void curve_mg_fp2_normalise_and_A24(curve_mg_fp2_t *E);

static inline void
curve_mg_fp2_AC_to_A24(xpoint_eqw_fp2_t *A24, const curve_mg_fp2_t *E)
{
    // Maybe we already have this computed
    if (E->is_A24_computed_and_normalized) {
        xpoint_eqw_fp2_copy(A24, &E->A24);
        return;
    }

    // A24 = (A+2C : 4C)
    fp2_add(&A24->z, &E->C, &E->C);
    fp2_add(&A24->x, &E->A, &A24->z);
    fp2_add(&A24->z, &A24->z, &A24->z);
}

// *** Fp ***
typedef struct curve_mg_fp_t { fp_t A; fp_t C; xpoint_eqw_fp_t A24; bool is_A24_computed_and_normalized; } curve_mg_fp_t;
typedef struct curve_sw_fp_t { fp_t A; fp_t B; fp_t C; } curve_sw_fp_t;

void curve_mg_fp_init(curve_mg_fp_t *E);
void xpoint_eqw_fp_init(xpoint_eqw_fp_t *P);

int curve_mg_fp_verify_A(const fp_t *A);
int curve_mg_fp_init_from_A(curve_mg_fp_t *E, const fp_t *A);

void curve_mg_fp_normalise(curve_mg_fp_t *E);
void curve_mg_fp_normalise_A24(curve_mg_fp_t *E);
void curve_mg_fp_normalise_and_A24(curve_mg_fp_t *E);

static inline void
curve_mg_fp_AC_to_A24(xpoint_eqw_fp_t *A24, const curve_mg_fp_t *E)
{
    // Maybe we already have this computed
    if (E->is_A24_computed_and_normalized) {
        xpoint_eqw_fp_copy(A24, &E->A24);
        return;
    }

    // A24 = (A+2C : 4C)
    fp_add(&A24->z, &E->C, &E->C);
    fp_add(&A24->x, &E->A, &A24->z);
    fp_add(&A24->z, &A24->z, &A24->z);
}


// --------------------------------------------------------------------------------
// Basis computation
// --------------------------------------------------------------------------------
// *** Fp2 ***
void curve_mg_fp2_2e_basis(point_eqw_fp2_t *P, point_eqw_fp2_t *Q, curve_mg_fp2_t *Et, curve_mg_fp2_t *E, uint32_t e);

// *** Fp ***
void curve_mg_fp_2e_basis(point_eqw_fp_t *P, point_eqw_fp_t *Q, curve_mg_fp_t *E, curve_mg_fp_t *Et, uint32_t e);

// --------------------------------------------------------------------------------
// Point membership
// --------------------------------------------------------------------------------

uint32_t x_on_curve_fp(fp_t *y, const fp_t *x, const curve_mg_fp_t *E);
uint32_t point_eqw_fp_on_curve(const point_eqw_fp_t *P, const curve_mg_fp_t *E);

// --------------------------------------------------------------------------------
// Point arithmetic
// --------------------------------------------------------------------------------

// *** Fp2 ***
uint32_t xpoint_eqw_fp2_is_equal(const xpoint_eqw_fp2_t *P, const xpoint_eqw_fp2_t *Q);
uint32_t xpoint_eqw_fp2_is_zero(const xpoint_eqw_fp2_t *P);
uint32_t xpoint_eqw_fp2_is_two_torsion(const xpoint_eqw_fp2_t *P, const curve_mg_fp2_t *E);

void _xpoint_eqw_fp2_dbl_E0(xpoint_eqw_fp2_t *Q, const xpoint_eqw_fp2_t *P);
void _xpoint_eqw_fp2_dbl_A24(xpoint_eqw_fp2_t *Q, const xpoint_eqw_fp2_t *P, const xpoint_eqw_fp2_t *A24, const bool A24_normalized);

void xpoint_eqw_fp2_add(xpoint_eqw_fp2_t *R, const xpoint_eqw_fp2_t *P, const xpoint_eqw_fp2_t *Q, const xpoint_eqw_fp2_t *PQ);
void xpoint_eqw_fp2_dbl(xpoint_eqw_fp2_t *res, const xpoint_eqw_fp2_t *P, const curve_mg_fp2_t *curve);
void xpoint_eqw_fp2_mul(xpoint_eqw_fp2_t *res, xpoint_eqw_fp2_t *resp1, const digit_t *scalar, const int kbits, const xpoint_eqw_fp2_t *P, curve_mg_fp2_t *curve);
void xpoint_eqw_fp2_mul_dbl_iter(xpoint_eqw_fp2_t *res, xpoint_eqw_fp2_t *resp1, const int k, const xpoint_eqw_fp2_t *P, curve_mg_fp2_t *curve);
void xpoint_eqw_fp2_dbl_iter(xpoint_eqw_fp2_t *res, int n, const xpoint_eqw_fp2_t *P, curve_mg_fp2_t *curve);

void point_eqw_fp2_neg(point_eqw_fp2_t *Q, point_eqw_fp2_t *P);
void point_eqw_fp2_mul(point_eqw_fp2_t *kP, digit_t const *k, uint32_t const kbits, const point_eqw_fp2_t *P, curve_mg_fp2_t *E);
void point_eqw_fp2_dbl_iter(point_eqw_fp2_t *kP, uint32_t const k, const point_eqw_fp2_t *P, curve_mg_fp2_t *E);

uint32_t xpoint_eqw_fp2_recover_y_sqrt(fp2_t *y, const fp2_t *Px, const curve_mg_fp2_t *curve);

uint32_t point_jac_fp2_is_equal(const point_jac_fp2_t *P, const point_jac_fp2_t *Q);

void point_jac_fp2_to_ec(xpoint_eqw_fp2_t *P, const point_jac_fp2_t *xyP);
void jac_to_ws_fp2(point_jac_fp2_t *P, fp2_t *t, fp2_t *ao3, const point_jac_fp2_t *Q, const curve_mg_fp2_t *curve);
void jac_from_ws_fp2(point_jac_fp2_t *Q, const point_jac_fp2_t *P, const fp2_t *ao3, const curve_mg_fp2_t *curve);

void point_jac_fp2_neg(point_jac_fp2_t *Q, const point_jac_fp2_t *P);
void point_jac_fp2_add(point_jac_fp2_t *R, const point_jac_fp2_t *P, const point_jac_fp2_t *Q, const curve_mg_fp2_t *AC);
void point_jac_fp2_dbl(point_jac_fp2_t *Q, const point_jac_fp2_t *P, const curve_mg_fp2_t *AC);
void point_jac_fp2_dblw(point_jac_fp2_t *Q, fp2_t *u, const point_jac_fp2_t *P, const fp2_t *t);
void point_jac_fp2_to_xz_add_components(add_components_fp2_t *uvw, const point_jac_fp2_t *P, const point_jac_fp2_t *Q, const curve_mg_fp2_t *AC);

void point_jac_fp2_mul(point_jac_fp2_t *Q, const digit_t *k, const uint32_t kbits, const point_jac_fp2_t *P, curve_mg_fp2_t *AC);
void point_jac_fp2_dbl_iter_iter(point_jac_fp2_t *Q, const point_jac_fp2_t *P, const uint32_t k, curve_mg_fp2_t *E);

uint32_t eqw_point_is_equal_fp2(const point_eqw_fp2_t *Q, const point_eqw_fp2_t *P);
uint32_t xpoint_eqw_fp2_is_equal(const xpoint_eqw_fp2_t *Q, const xpoint_eqw_fp2_t *P);

void point_eqw_fp2_cswap(point_eqw_fp2_t *P, point_eqw_fp2_t *Q, const digit_t option);

// Short Weierstrass curves
void eqw_sw_dbl_fp2(point_eqw_fp2_t *Q, const point_eqw_fp2_t *P, const curve_sw_fp2_t *E);
void eqw_sw_xdbl_fp2(xpoint_eqw_fp2_t *Q, const xpoint_eqw_fp2_t *P, const curve_sw_fp2_t *E);
void eqw_sw_add_fp2(point_eqw_fp2_t *Q, const point_eqw_fp2_t *P, const curve_sw_fp2_t *E);

void sw_curve_normalise_fp2(curve_sw_fp2_t *E2, const curve_sw_fp2_t *E1);
void sw_xdouble_fp2(xpoint_eqw_fp2_t *two_P, const curve_sw_fp2_t *E, const fp2_t *C_pow2, const fp2_t *C_pow3, const xpoint_eqw_fp2_t *P);
void sw_eqw_2isog_eval_fp2(point_eqw_fp2_t *Q, const xpoint_eqw_fp2_t *K, const point_eqw_fp2_t *P, const fp2_t *T, const fp2_t *C_pow2);
void sw_eqw_2isog_xeval_fp2(xpoint_eqw_fp2_t *Qxz, const xpoint_eqw_fp2_t *K, const xpoint_eqw_fp2_t *P, const fp2_t *T, const fp2_t *C_pow2);
void sw_eqw_2isog_chain_eval_fp2(curve_sw_fp2_t *E2, point_eqw_fp2_t *E2_pts, const curve_sw_fp2_t *E1, const xpoint_eqw_fp2_t *K, const uint32_t log_2_K_order, const point_eqw_fp2_t *E1_pts, const uint32_t len_pts);

// *** Fp ***
uint32_t xpoint_eqw_fp_is_equal(const xpoint_eqw_fp_t *P, const xpoint_eqw_fp_t *Q);
uint32_t xpoint_eqw_fp_is_zero(const xpoint_eqw_fp_t *P);
uint32_t xpoint_eqw_fp_is_two_torsion(const xpoint_eqw_fp_t *P, const curve_mg_fp_t *E);

void _xpoint_eqw_fp_dbl_E0(xpoint_eqw_fp_t *Q, const xpoint_eqw_fp_t *P);
void _xpoint_eqw_fp_dbl_A24(xpoint_eqw_fp_t *Q, const xpoint_eqw_fp_t *P, const xpoint_eqw_fp_t *A24, const bool A24_normalized);

void xpoint_eqw_fp_add(xpoint_eqw_fp_t *R, const xpoint_eqw_fp_t *P, const xpoint_eqw_fp_t *Q, const xpoint_eqw_fp_t *PQ);
void xpoint_eqw_fp_dbl(xpoint_eqw_fp_t *res, const xpoint_eqw_fp_t *P, const curve_mg_fp_t *curve);
void xpoint_eqw_fp_mul(xpoint_eqw_fp_t *res, xpoint_eqw_fp_t *resp1, const digit_t *scalar, const int kbits, const xpoint_eqw_fp_t *P, curve_mg_fp_t *curve);
void xpoint_eqw_fp_mul_dbl_iter(xpoint_eqw_fp_t *res, xpoint_eqw_fp_t *resp1, const int k, const xpoint_eqw_fp_t *P, curve_mg_fp_t *curve);
void xpoint_eqw_fp_dbl_iter(xpoint_eqw_fp_t *res, int n, const xpoint_eqw_fp_t *P, curve_mg_fp_t *curve);

void point_eqw_fp_neg(point_eqw_fp_t *Q, point_eqw_fp_t *P);
void point_eqw_fp_mul(point_eqw_fp_t *kP, digit_t const *k, uint32_t const kbits, const point_eqw_fp_t *P, curve_mg_fp_t *E);
void point_eqw_fp_dbl_iter(point_eqw_fp_t *kP, uint32_t const k, const point_eqw_fp_t *P, curve_mg_fp_t *E);

void xpoint_eqw_fp_recover_y(point_eqw_fp_t *Q, point_eqw_fp_t *P, const xpoint_eqw_fp_t *Qx, const xpoint_eqw_fp_t *PpQx, curve_mg_fp_t *E);
uint32_t xpoint_eqw_fp_recover_y_sqrt(fp_t *y, const fp_t *Px, const curve_mg_fp_t *curve);

uint32_t point_jac_fp_is_equal(const point_jac_fp_t *P, const point_jac_fp_t *Q);

void point_jac_fp_to_ec(xpoint_eqw_fp_t *P, const point_jac_fp_t *xyP);
void jac_to_ws_fp(point_jac_fp_t *P, fp_t *t, fp_t *ao3, const point_jac_fp_t *Q, const curve_mg_fp_t *curve);
void jac_from_ws_fp(point_jac_fp_t *Q, const point_jac_fp_t *P, const fp_t *ao3, const curve_mg_fp_t *curve);

void point_jac_fp_neg(point_jac_fp_t *Q, const point_jac_fp_t *P);
void point_jac_fp_add(point_jac_fp_t *R, const point_jac_fp_t *P, const point_jac_fp_t *Q, const curve_mg_fp_t *AC);
void point_jac_fp_dbl(point_jac_fp_t *Q, const point_jac_fp_t *P, const curve_mg_fp_t *AC);
void point_jac_fp_dblw(point_jac_fp_t *Q, fp_t *u, const point_jac_fp_t *P, const fp_t *t);
void point_jac_fp_to_xz_add_components(add_components_fp_t *uvw, const point_jac_fp_t *P, const point_jac_fp_t *Q, const curve_mg_fp_t *AC);

void point_jac_fp_mul(point_jac_fp_t *Q, const digit_t *k, const uint32_t kbits, const point_jac_fp_t *P, curve_mg_fp_t *AC);
void point_jac_fp_dbl_iter_iter(point_jac_fp_t *Q, const point_jac_fp_t *P, const uint32_t k, curve_mg_fp_t *E);

uint32_t eqw_point_is_equal_fp(const point_eqw_fp_t *Q, const point_eqw_fp_t *P);
uint32_t xpoint_eqw_fp_is_equal(const xpoint_eqw_fp_t *Q, const xpoint_eqw_fp_t *P);

void point_eqw_fp_cswap(point_eqw_fp_t *P, point_eqw_fp_t *Q, const digit_t option);

// Short Weierstrass curves
void eqw_sw_dbl_fp(point_eqw_fp_t *Q, const point_eqw_fp_t *P, const curve_sw_fp_t *E);
void eqw_sw_xdbl_fp(xpoint_eqw_fp_t *Q, const xpoint_eqw_fp_t *P, const curve_sw_fp_t *E);
void eqw_sw_add_fp(point_eqw_fp_t *Q, const point_eqw_fp_t *P, const curve_sw_fp_t *E);

void sw_curve_normalise_fp(curve_sw_fp_t *E2, const curve_sw_fp_t *E1);
void sw_xdouble_fp(xpoint_eqw_fp_t *two_P, const curve_sw_fp_t *E, const fp_t *C_pow2, const fp_t *C_pow3, const xpoint_eqw_fp_t *P);
void sw_eqw_2isog_eval_fp(point_eqw_fp_t *Q, const xpoint_eqw_fp_t *K, const point_eqw_fp_t *P, const fp_t *T, const fp_t *C_pow2);
void sw_eqw_2isog_xeval_fp(xpoint_eqw_fp_t *Qxz, const xpoint_eqw_fp_t *K, const xpoint_eqw_fp_t *P, const fp_t *T, const fp_t *C_pow2);
void sw_eqw_2isog_chain_eval_fp(curve_sw_fp_t *E2, point_eqw_fp_t *E2_pts, const curve_sw_fp_t *E1, const xpoint_eqw_fp_t *K, const uint32_t log_2_K_order, const point_eqw_fp_t *E1_pts, const uint32_t len_pts);

// --------------------------------------------------------------------------------
// Conversions
// --------------------------------------------------------------------------------
void point_ewq_fp_select(point_eqw_fp_t *Q, const point_eqw_fp_t *P1, const point_eqw_fp_t *P2, const digit_t option);

void point_eqw_to_jac_fp_to_fp2(point_jac_fp2_t *P_jac, const point_eqw_fp_t *P_eqw);

static inline void
point_eqw_fp_to_fp2(point_eqw_fp2_t *Q, const point_eqw_fp_t *P)
{
    fp_copy(&Q->x.re, &P->x);
    fp_set_zero(&Q->x.im);
    fp_copy(&Q->y.re, &P->y);
    fp_set_zero(&Q->y.im);
    fp_copy(&Q->z.re, &P->z);
    fp_set_zero(&Q->z.im);
}

static inline void
point_eqw_fp2_to_fp(point_eqw_fp_t *Q, const point_eqw_fp2_t *P)
{
    assert(fp_is_zero(&P->x.im));
    assert(fp_is_zero(&P->y.im));
    assert(fp_is_zero(&P->z.im));
    fp_copy(&Q->x, &P->x.re);
    fp_copy(&Q->y, &P->y.re);
    fp_copy(&Q->z, &P->z.re);
}

static inline void
xpoint_eqw_fp_to_fp2(xpoint_eqw_fp2_t *Q, const xpoint_eqw_fp_t *P)
{
    fp_copy(&Q->x.re, &P->x);
    fp_set_zero(&Q->x.im);
    fp_copy(&Q->z.re, &P->z);
    fp_set_zero(&Q->z.im);
}

static inline void
xpoint_eqw_fp2_to_fp(xpoint_eqw_fp_t *Q, const xpoint_eqw_fp2_t *P)
{
    assert(fp_is_zero(&P->x.im));
    assert(fp_is_zero(&P->z.im));
    fp_copy(&Q->x, &P->x.re);
    fp_copy(&Q->z, &P->z.re);
}

static inline void
curve_mg_fp_to_fp2(curve_mg_fp2_t *E_fp2, curve_mg_fp_t *E_fp) {
    curve_mg_fp2_init(E_fp2);
    fp_copy(&E_fp2->A.re, &E_fp->A);
    fp_copy(&E_fp2->C.re, &E_fp->C);
    E_fp2->is_A24_computed_and_normalized = E_fp->is_A24_computed_and_normalized;
    xpoint_eqw_fp_to_fp2(&E_fp2->A24, &E_fp->A24);
}

static inline void
curve_mg_fp2_to_fp(curve_mg_fp_t *E_fp, curve_mg_fp2_t *E_fp2) {
    curve_mg_fp_init(E_fp);
    assert(fp_is_zero(&E_fp2->A.im));
    assert(fp_is_zero(&E_fp2->C.im));
    fp_copy(&E_fp->A, &E_fp2->A.re);
    fp_copy(&E_fp->C, &E_fp2->C.re);
    E_fp->is_A24_computed_and_normalized = E_fp2->is_A24_computed_and_normalized;
    xpoint_eqw_fp2_to_fp(&E_fp->A24, &E_fp2->A24);
}

// *** Fp2 ***
void point_jac_to_eqw_fp2(point_eqw_fp2_t *P_eqw, const point_jac_fp2_t *P_jac);
void point_eqw_to_jac_fp2(point_jac_fp2_t *P_jac, const point_eqw_fp2_t *P_eqw);
void mg_to_sw_fp2(curve_sw_fp2_t *E_sw, xpoint_eqw_fp2_t *xpts_sw, point_eqw_fp2_t *pts_sw, const curve_mg_fp2_t *E_m, const xpoint_eqw_fp2_t *xpts_m, const uint32_t len_xpts, const point_eqw_fp2_t *pts_m, const uint32_t len_pts);
void sw_to_mg_fp2(curve_mg_fp2_t *E_m, xpoint_eqw_fp2_t *xpts_m, point_eqw_fp2_t *pts_m, const curve_sw_fp2_t *E_sw, const xpoint_eqw_fp2_t *xT8, const xpoint_eqw_fp2_t *xpts_sw, const uint32_t len_xpts, const point_eqw_fp2_t *pts_sw, const uint32_t len_pts);

void point_eqw_fp2_to_xpoint(xpoint_eqw_fp2_t *Px, const point_eqw_fp2_t *P);

// *** Fp ***
void point_jac_to_eqw_fp(point_eqw_fp_t *P_eqw, const point_jac_fp_t *P_jac);
void point_eqw_to_jac_fp(point_jac_fp_t *P_jac, const point_eqw_fp_t *P_eqw);
void mg_to_sw_fp(curve_sw_fp_t *E_sw, xpoint_eqw_fp_t *xpts_sw, point_eqw_fp_t *pts_sw, const curve_mg_fp_t *E_m, const xpoint_eqw_fp_t *xpts_m, const uint32_t len_xpts, const point_eqw_fp_t *pts_m, const uint32_t len_pts);
void sw_to_mg_fp(curve_mg_fp_t *E_m, xpoint_eqw_fp_t *xpts_m, point_eqw_fp_t *pts_m, const curve_sw_fp_t *E_sw, const xpoint_eqw_fp_t *xT8, const xpoint_eqw_fp_t *xpts_sw, const uint32_t len_xpts, const point_eqw_fp_t *pts_sw, const uint32_t len_pts);

void point_eqw_fp_to_xpoint(xpoint_eqw_fp_t *Px, const point_eqw_fp_t *P);

// --------------------------------------------------------------------------------
// Test functions for printing and order checking, only used in debug mode
// --------------------------------------------------------------------------------

static void
point_eqw_fp_print(point_eqw_fp_t *P) {
    fp_print("P.x = ", &P->x);
    fp_print("P.y = ", &P->y);
    fp_print("P.z = ", &P->z);
}

// *** Fp2 ***
static inline void
curve_mg_fp2_copy(curve_mg_fp2_t *E1, const curve_mg_fp2_t *E2) {
    fp2_copy(&(E1->A), &(E2->A));
    fp2_copy(&(E1->C), &(E2->C));
    E1->is_A24_computed_and_normalized = E2->is_A24_computed_and_normalized;
    xpoint_eqw_fp2_copy(&E1->A24, &E2->A24);
}

/**
 * @brief Check if a point (X : Z) has order exactly 2^t
 *
 * @param P: a point
 * @param E: an elliptic curve
 * @param t: an integer
 *
 * @return 0xFFFFFFFF if the order is correct, 0 otherwise
 */
static int
xpoint_eqw_fp2_test_order_twof(const xpoint_eqw_fp2_t *P, const curve_mg_fp2_t *E, int t)
{
    xpoint_eqw_fp2_t test;
    curve_mg_fp2_t curve;
    test = *P;
    curve_mg_fp2_copy(&curve, E);

    if (xpoint_eqw_fp2_is_zero(&test))
        return 0;
    // Scale point by 2^(t-1)
    xpoint_eqw_fp2_dbl_iter(&test, t - 1, &test, &curve);
    // If it's zero now, it doesnt have order 2^t
    if (xpoint_eqw_fp2_is_zero(&test)) {
        printf("Order < 2^%d\n", t);
        return 0;
    }
    // Ensure [2^t] P = 0
    xpoint_eqw_fp2_dbl(&test, &test, &curve);

    if (!xpoint_eqw_fp2_is_zero(&test)) {
        int extra_order = 0;
        while (!xpoint_eqw_fp2_is_zero(&test) && extra_order < 32) {
            xpoint_eqw_fp2_dbl(&test, &test, &curve);
            extra_order++;
        }
        if (extra_order == 32) {
            printf("Order > 2^(%d + extra_order)\n", extra_order);
        } else {
            printf("Order == 2^(%d + extra_order)\n", extra_order);
        }
        return 0;
    }

    return xpoint_eqw_fp2_is_zero(&test);
}


static int
point_jac_fp2_test_order_twof(const point_jac_fp2_t *P, const curve_mg_fp2_t *E, int t)
{
    point_jac_fp2_t test;
    test = *P;
    if (fp2_is_zero(&test.z))
        return 0;
    for (int i = 0; i < t - 1; i++) {
        point_jac_fp2_dbl(&test, &test, E);
    }
    if (fp2_is_zero(&test.z))
        return 0;
    point_jac_fp2_dbl(&test, &test, E);
    return (fp2_is_zero(&test.z));
}


static uint32_t
point_eqw_fp2_on_mg_curve(point_eqw_fp2_t const *P, curve_mg_fp2_t const *curve)
{
    // Tests wether `P = (x: y: z)` (in equally weighted coordinates) lies on
    // `curve` given by y^2 = f(x) = x(x^2 + A + 1)
    fp2_t x, y, z_inv, y_sqr, A_x_sqr, x_pow3, f_x;

    // Normalise points
    fp2_copy(&z_inv, &P->z);
    fp2_inv(&z_inv);
    fp2_mul(&x, &P->x, &z_inv);
    fp2_mul(&y, &P->y, &z_inv);

    fp2_sqr(&y_sqr, &y);

    fp2_sqr(&A_x_sqr, &x);
    fp2_mul(&A_x_sqr, &A_x_sqr, &curve->A);

    fp2_sqr(&x_pow3, &x);
    fp2_mul(&x_pow3, &x_pow3, &x);

    fp2_add(&f_x, &x_pow3, &A_x_sqr);
    fp2_add(&f_x, &f_x, &x);

    return fp2_is_equal(&y_sqr, &f_x);
}

// *** Fp ***
static inline void
curve_mg_fp_copy(curve_mg_fp_t *E1, const curve_mg_fp_t *E2) {
    fp_copy(&(E1->A), &(E2->A));
    fp_copy(&(E1->C), &(E2->C));
    E1->is_A24_computed_and_normalized = E2->is_A24_computed_and_normalized;
    xpoint_eqw_fp_copy(&E1->A24, &E2->A24);
}

/**
 * @brief Check if a point (X : Z) has order exactly 2^t
 *
 * @param P: a point
 * @param E: an elliptic curve
 * @param t: an integer
 *
 * @return 0xFFFFFFFF if the order is correct, 0 otherwise
 */
static int
xpoint_eqw_fp_test_order_twof(const xpoint_eqw_fp_t *P, const curve_mg_fp_t *E, int t)
{
    xpoint_eqw_fp_t test;
    curve_mg_fp_t curve;
    test = *P;
    curve_mg_fp_copy(&curve, E);

    if (xpoint_eqw_fp_is_zero(&test))
        return 0;
    // Scale point by 2^(t-1)
    xpoint_eqw_fp_dbl_iter(&test, t - 1, &test, &curve);
    // If it's zero now, it doesnt have order 2^t
    if (xpoint_eqw_fp_is_zero(&test)) {
        printf("Order < 2^%d\n", t);
        return 0;
    }
    // Ensure [2^t] P = 0
    xpoint_eqw_fp_dbl(&test, &test, &curve);

    if (!xpoint_eqw_fp_is_zero(&test)) {
        int extra_order = 0;
        while (!xpoint_eqw_fp_is_zero(&test) && extra_order < 32) {
            xpoint_eqw_fp_dbl(&test, &test, &curve);
            extra_order++;
        }
        if (extra_order == 32) {
            printf("Order > 2^(%d + extra_order)\n", extra_order);
        } else {
            printf("Order == 2^(%d + extra_order)\n", extra_order);
        }
        return 0;
    }

    return xpoint_eqw_fp_is_zero(&test);
}


static int
point_jac_fp_test_order_twof(const point_jac_fp_t *P, const curve_mg_fp_t *E, int t)
{
    point_jac_fp_t test;
    test = *P;
    if (fp_is_zero(&test.z))
        return 0;
    for (int i = 0; i < t - 1; i++) {
        point_jac_fp_dbl(&test, &test, E);
    }
    if (fp_is_zero(&test.z))
        return 0;
    point_jac_fp_dbl(&test, &test, E);
    return (fp_is_zero(&test.z));
}


static uint32_t
point_eqw_fp_on_mg_curve(point_eqw_fp_t const *P, curve_mg_fp_t const *curve)
{
    // Tests wether `P = (x: y: z)` (in equally weighted coordinates) lies on
    // `curve` given by y^2 = f(x) = x(x^2 + A + 1)
    fp_t x, y, z_inv, y_sqr, A_x_sqr, x_pow3, f_x;

    // Normalise points
    fp_copy(&z_inv, &P->z);
    fp_inv(&z_inv);
    fp_mul(&x, &P->x, &z_inv);
    fp_mul(&y, &P->y, &z_inv);

    fp_sqr(&y_sqr, &y);

    fp_sqr(&A_x_sqr, &x);
    fp_mul(&A_x_sqr, &A_x_sqr, &curve->A);

    fp_sqr(&x_pow3, &x);
    fp_mul(&x_pow3, &x_pow3, &x);

    fp_add(&f_x, &x_pow3, &A_x_sqr);
    fp_add(&f_x, &f_x, &x);

    return fp_is_equal(&y_sqr, &f_x);
}

void point_jac_fp2_print(point_jac_fp2_t *P);
#endif
