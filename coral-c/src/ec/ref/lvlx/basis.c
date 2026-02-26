#include <assert.h>

#include "e0_basis.h"
#include "ec.h"
#include "fp2.h"
#include "fp.h"
#include "torsion_constants.h"

// Function adapted from github.com/Threshold-Group-Actions/qt_Pegasis_C
uint32_t
tate_2(
    const point_eqw_fp_t *P,
    const point_eqw_fp_t *Q,
    const bool normalized
){
	// Takes a 2-torsion point P and Q any other point (Q != P)
	// Returns 0 if tate_2(P, Q) = 1 and 0xF...F if tate_2(P, Q) = -1
	// Cost: 1I + 1Exp + 4M + 1a

    // tate_2(P, Q) "=" is_square((Qx * Pz - Qz * Px) / (Qz * Pz))

	if (normalized) {
        // Assumes Pz == Qz == 1
		fp_t t0;
		fp_sub(&t0, &Q->x, &P->x);
		return ~fp_is_square(&t0);
	} else {
		fp_t t0, t1;
        // t0 = Qx * Pz - Qz * Px
		fp_mul(&t0, &Q->x, &P->z);
		fp_mul(&t1, &Q->z, &P->x);
		fp_sub(&t0, &t0, &t1);
        // t1 = z * z_P

		fp_mul(&t1, &Q->z, &P->z);
		fp_inv(&t1);
		fp_mul(&t0, &t0, &t1);
		return ~fp_is_square(&t0);
	}
}

// Function adapted from github.com/Threshold-Group-Actions/qt_Pegasis_C
uint32_t
self_tate_2(
        point_eqw_fp_t *P,
        const curve_mg_fp_t *E
) {
	// Returns 0 if tate_2(P, P) = 1 and 0xF..F if tate_2(P, P) = -1
	// (Here P is a 2-torsion point)
    //
	// Uses the formula
    //   t_2(P, P) = f_{2, P}(D_P)^{(p-1) / 2}
	// with
    //
    //   f_{2, P}(D_P)
    //   = lt_P(f_{2, P})
    //   = lt_P(x - Px)
    //   = 1 / (3 * Px^2 + 2 * A * Px + 1)
    //   = -1 / (A * Px + 2)
    //
    //  when x_P != 0 (with x_P normalised)

	// Cost: 1Exp + 1M + 2a (+1 I)
	// Not constant time but used deterministically independently of secrets

	fp_t t0, t1;
	uint32_t ret;

	// Normalize
	if (!fp_is_one(&P->z)) {
		fp_inv(&P->z);
		fp_mul(&P->x, &P->x, &P->z);
		fp_set_one(&P->z);
	}

    // t0 = A * Px
	fp_mul(&t0, &P->x, &E->A);
    // t1 = 2
	fp_set_small(&t1, 2);
    // t0 = A * Px + 2 = -(3 * x_P^2 + 2 * A * Px + 1)
	fp_add(&t0, &t0, &t1);

    // -1 is not a square in Fp
	ret = fp_is_square(&t0);

    // if Px == 0, tate_2(P, P) must be 1 (return 0)
	return ret ^ (ret & fp_is_zero(&P->x));
}


// Function adapted from github.com/Threshold-Group-Actions/qt_Pegasis_C
void
two_torsion_fp(
        point_eqw_fp_t *T0,
        point_eqw_fp_t *T1,
        point_eqw_fp_t *Tm1,
        const curve_mg_fp_t *E
){
	// Ref/ Notation: PEGASIS (2025/401), Lemmas D.1/D.2
    // Returned points are normalised

	fp_t delta, four;
	point_eqw_fp_t P0, P1, Pm1;
	uint32_t sel0, sel1;

	// Point (0 : 0 : 1)
	fp_set_zero(&P0.x);
	fp_set_zero(&P0.y);
	fp_set_one(&P0.z);

    // delta = sqrt(A^2 - 4)
	fp_sqr(&delta, &E->A);
	fp_set_small(&four, 4);
	fp_sub(&delta, &delta, &four);
	fp_sqrt(&delta);

    // alpha = (-A + delta) / 2
	// Point (alpha : 0 : 1)
	fp_sub(&P1.x, &delta, &E->A);
	fp_half(&P1.x, &P1.x);
	fp_set_zero(&P1.y);
	fp_set_one(&P1.z);

	// Point (1 / alpha : 0 : 1)
    // Note 1 / alpha = -(A + delta)/2 = alpha - delta
	fp_sub(&Pm1.x, &P1.x, &delta);
	fp_set_zero(&Pm1.y);
	fp_set_one(&Pm1.z);

	// Find T0 (among P1 and Pm1 because tate_2(P0, P0) = 1)
	sel0 = self_tate_2(&P1, E);
    // tate_2(T0, T0) = -1
	point_ewq_fp_select(T0, &Pm1, &P1, sel0);
    // Don't know yet if T1 = T1 or Tm1
	point_ewq_fp_select(T1, &Pm1, &P1, ~sel0);

	// Distinguish T1 and Tm1
	point_eqw_fp_copy(Tm1, &P0);
	sel1 = tate_2(T1, T0, true);
    // tate_2(T1, T0) = -1 and tate_2(Tm1, T0) = 1
	point_eqw_fp_cswap(Tm1, T1, ~sel1);

    assert(point_eqw_fp_on_curve(T0, E));
    assert(point_eqw_fp_on_curve(T1, E));
    assert(point_eqw_fp_on_curve(Tm1, E));
}


// Function adapted from github.com/Threshold-Group-Actions/qt_Pegasis_C
void
ec_twist_fp(curve_mg_fp_t *Et, const curve_mg_fp_t *E) {
    fp_neg(&Et->A, &E->A);
    fp_copy(&Et->C, &E->C);

    xpoint_eqw_fp_copy(&Et->A24, &E->A24);
    Et->is_A24_computed_and_normalized = E->is_A24_computed_and_normalized;

    // Et.A24 = (-A + 2C : 4C) = (-(A + 2C) + 4C : 4C)
    fp_neg(&Et->A24.x, &E->A24.x);
    fp_add(&Et->A24.x, &Et->A24.x, &E->A24.z);
}


// Function adapted from github.com/Threshold-Group-Actions/qt_Pegasis_C
void
curve_mg_fp_2e_basis(
        point_eqw_fp_t *P,
        point_eqw_fp_t *Q,
        curve_mg_fp_t *Et,
        curve_mg_fp_t *E,
        uint32_t e
){
	// Returns P in E(Fp) and Q in Et(Fp) each of order 2^e
	// Where E: y^2 = x^3 + Ax^2 + x and E^t: y^2 = x^3 - Ax^2 + x

    fp_t _, __;
    // clock_t start;

	ec_twist_fp(Et, E);

	point_eqw_fp_t T0, T1, Tm1;
	fp_t Px, Py, Qx, Qy, x, y;
	uint32_t sel;

	two_torsion_fp(&T0, &T1, &Tm1, E);
    fp_t nqr;

        // start = clock();
    // Choice of P
    fp_set_zero(&Px);
    fp_set_zero(&Py);
    for (uint32_t i = 0; i < 20; i++) {
        fp_set_small(&nqr, FP_nqr[i]);
        fp_add(&x, &nqr, &Tm1.x);
        sel = x_on_curve_fp(&y, &x, E);
        fp_select(&Px, &Px, &x, sel);
        fp_select(&Py, &Py, &y, sel);
        // Does not need to be CT, it is public data
        if (sel) break;
    }
        assert(sel);
        // printf(":: [Basis sampling] Picking P %.3Lf ms\n", (long double)(clock() - start) * 1000 / CLOCKS_PER_SEC);

        // start = clock();
    fp_copy(&P->x, &Px);
    fp_sqrt(&Py);
    fp_copy(&P->y, &Py);
    fp_set_one(&P->z);
    assert(point_eqw_fp_on_curve(P, E));

    // Need to clear the cofactors and reduce to the right power of 2
    // Optimise: Could combine so that we don't need to recover_y twice
    point_eqw_fp_mul(P, p_cofactor_for_2f, P_COFACTOR_FOR_2F_BITLENGTH, P, E);
    point_eqw_fp_dbl_iter(P, TORSION_EVEN_POWER - 1 - e, P, E);

    #ifndef NDEBUG
        point_eqw_fp_t _P;
        point_eqw_fp_copy(&_P, P);
        point_eqw_fp_normalise(&_P);
        assert(x_on_curve_fp(&_, &_P.x, E));
        assert(point_eqw_fp_on_curve(&_P, E));
    #endif
        // printf(":: [Basis sampling] Scaling P %.3Lf ms\n", (long double)(clock() - start) * 1000 / CLOCKS_PER_SEC);

        // start = clock();
    // Choice of Q
    fp_set_zero(&Qx);
    fp_set_zero(&Qy);
    for (uint32_t i = 0; i < 20; i++) {
        fp_set_small(&nqr, FP_nqr[i]);
        fp_sub(&x, &nqr, &T1.x);
        // fp_sub(&x, &T1.x, FP_nqr + i);
        sel = x_on_curve_fp(&y, &x, Et);
        fp_select(&Qx, &Qx, &x, sel);
        fp_select(&Qy, &Qy, &y, sel);
        // Does not need to be CT, it is public data
        if (sel) break;
    }
        assert(sel);
        // printf(":: [Basis sampling] Picking Q %.3Lf ms\n", (long double)(clock() - start) * 1000 / CLOCKS_PER_SEC);

        // start = clock();
    fp_copy(&Q->x, &Qx);
    fp_sqrt(&Qy);
    fp_copy(&Q->y, &Qy);
    fp_set_one(&Q->z);
    assert(point_eqw_fp_on_curve(Q, Et));

    // Need to clear the cofactors and reduce to the right power of 2
    // Optimise: Could combine so that we don't need to recover_y twice
    point_eqw_fp_mul(Q, p_cofactor_for_2f, P_COFACTOR_FOR_2F_BITLENGTH, Q, Et);
    point_eqw_fp_dbl_iter(Q, TORSION_EVEN_POWER - 1 - e, Q, Et);

    #ifndef NDEBUG
        point_eqw_fp_t _Q;
        point_eqw_fp_copy(&_Q, Q);
        point_eqw_fp_normalise(&_Q);
        assert(x_on_curve_fp(&_, &_Q.x, Et));
        assert(point_eqw_fp_on_curve(&_Q, Et));
    #endif
        // printf(":: [Basis sampling] Scaling Q %.3Lf ms\n", (long double)(clock() - start) * 1000 / CLOCKS_PER_SEC);

    // Stop the compiler complaining about unused values
    (void)_;
    (void)__;
}

uint32_t
xpoint_eqw_fp2_recover_y_sqrt(fp2_t *y, const fp2_t *Px, const curve_mg_fp2_t *curve)
{ // Recover y-coordinate of a point on the Montgomery curve y^2 = x^3 + Ax^2 + x
    fp2_t t0;

    fp2_sqr(&t0, Px);
    fp2_mul(y, &t0, &curve->A); // Ax^2
    fp2_add(y, y, Px);          // Ax^2 + x
    fp2_mul(&t0, &t0, Px);
    fp2_add(y, y, &t0); // x^3 + Ax^2 + x
    // This is required, because we do not yet know that our curves are
    // supersingular so our points live on the twist with B = 1.
    return fp2_sqrt_verify(y);
}
