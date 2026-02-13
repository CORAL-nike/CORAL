#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "ec.h"
#include "mp.h"

void
point_ewq_fp_select(point_eqw_fp_t *Q, const point_eqw_fp_t *P1, const point_eqw_fp_t *P2, const digit_t option)
{
    // Select points
    // If option = 0 then Q <- P1, else if option = 0xFF...FF then Q <- P2
    fp_select(&(Q->x), &(P1->x), &(P2->x), option);
    fp_select(&(Q->y), &(P1->y), &(P2->y), option);
    fp_select(&(Q->z), &(P1->z), &(P2->z), option);
}

void
xpoint_eqw_fp_recover_y(
    point_eqw_fp_t *Q,
    point_eqw_fp_t *P,
    const xpoint_eqw_fp_t *Qx,
    const xpoint_eqw_fp_t *PpQx,
    curve_mg_fp_t *E
) {
    // Ref: https://eprint.iacr.org/2017/212, Algorithm 5
    // Modified to accept a non-normalised projecive point P
    // Cost: 13M + 1S + 5a + 3s

    // (Curve should already be normalised in CORAL)
    curve_mg_fp_normalise(E);

    fp_t PxQx, PxQz, PzQx, PzQz, APzQz_mul2;

    fp_mul(&PxQx, &P->x, &Qx->x);
    fp_mul(&PxQz, &P->x, &Qx->z);
    fp_mul(&PzQx, &P->z, &Qx->x);
    fp_mul(&PzQz, &P->z, &Qx->z);
    fp_mul(&APzQz_mul2, &E->A, &PzQz);
        fp_add(&APzQz_mul2, &APzQz_mul2, &APzQz_mul2);

    // More variables for clarity
    fp_t t1, t2, t3, t4, t5, t6, t7;

    // t1 = PxQx + PzQz
    // t2 = PxQz + PzQx + APzQz_mul2
    // t3 = PzQz * APzQz_mul2
    // t4 = (PxQz - PzQx) ** 2
    // t5 = PpQz * (t1 * t2 - t3)
    // t6 = t5 - t4 * PpQx
    // t7 = 2 * PpQz * Py * PzQz
    //
    // => t6 = PpQz * ((PxQx + PzQz) * (PxQz + PzQx + APzQz_mul2) - PzQz * APzQz_mul2) - (PxQz - PzQx)**2 * PpQx
    // => t7 = 2 * PpQz * Py * PzQz
    // Q = (Qx * t7, t6, Qz * t7)

    fp_add(&t1, &PxQx, &PzQz);
    fp_add(&t2, &PxQz, &PzQx);
        fp_add(&t2, &t2, &APzQz_mul2);
    fp_mul(&t3, &PzQz, &APzQz_mul2);
    fp_sub(&t4, &PxQz, &PzQx);
        fp_sqr(&t4, &t4);
    fp_mul(&t5, &t1, &t2);
        fp_sub(&t5, &t5, &t3);
        fp_mul(&t5, &t5, &PpQx->z);
    fp_mul(&t6, &t4, &PpQx->x);
        fp_sub(&t6, &t5, &t6);
    fp_mul(&t7, &PpQx->z, &P->y);
        fp_mul(&t7, &t7, &PzQz);
        fp_add(&t7, &t7, &t7);

    fp_mul(&Q->x, &Qx->x, &t7);
    fp_copy(&Q->y, &t6);
    fp_mul(&Q->z, &Qx->z, &t7);

    // // Original algorithm that does not accept non-normalised point
    // // Cost: 11M + 1S + 5a + 3s
    // point_eqw_fp_normalise(P);
    // fp_t v1, v2, v3, v4;
    //
    // fp_mul(&v1, &P->x, &Qx->z);
    // fp_add(&v2, &Qx->x, &v1);
    // fp_sub(&v3, &Qx->x, &v1);
    //
    // fp_sqr(&v3, &v3);
    // fp_mul(&v3, &v3, &PpQx->x);
    // fp_mul(&v1, &E->A, &Qx->z);
    // fp_add(&v1, &v1, &v1);
    //
    // fp_add(&v2, &v2, &v1);
    // fp_mul(&v4, &P->x, &Qx->x);
    // fp_add(&v4, &v4, &Qx->z);
    //
    // fp_mul(&v2, &v2, &v4);
    // fp_mul(&v1, &v1, &Qx->z);
    // fp_sub(&v2, &v2, &v1);
    //
    // fp_mul(&v2, &v2, &PpQx->z);
    // fp_sub(&Q->y, &v2, &v3);
    // // Our curve models enforce B == 1
    // fp_add(&v1, &P->y, &P->y);
    //
    // fp_mul(&v1, &v1, &Qx->z);
    // fp_mul(&v1, &v1, &PpQx->z);
    // fp_mul(&Q->x, &v1, &Qx->x);
    //
    // fp_mul(&Q->z, &v1, &Qx->z);

    #ifndef NDEBUG
        point_eqw_fp_t _Q;
        point_eqw_fp_copy(&_Q, Q);
        point_eqw_fp_normalise(&_Q);
        assert(point_eqw_fp_on_curve(&_Q, E));
    #endif
}

// --------------------------------------------------------------------------------
// Points
// --------------------------------------------------------------------------------

// Initialize point as identity element (0 : 1 : 0)
void point_eqw_fp_init(point_eqw_fp_t *P) { fp_set_zero(&(P->x)); fp_set_one(&(P->y)); fp_set_zero(&(P->z)); }
inline void point_eqw_fp_copy(point_eqw_fp_t *P, const point_eqw_fp_t *Q) { fp_copy(&P->x, &Q->x); fp_copy(&P->y, &Q->y); fp_copy(&P->z, &Q->z); }

void
point_eqw_fp_normalise(point_eqw_fp_t *P)
{
    if (!fp_is_one(&P->z)) {
        fp_inv(&P->z);
        fp_mul(&P->x, &P->x, &P->z);
        fp_mul(&P->y, &P->y, &P->z);
        fp_set_one(&(P->z));
    }
}

uint32_t
eqw_point_is_equal_fp(const point_eqw_fp_t *Q, const point_eqw_fp_t *P)
{
    point_eqw_fp_t _Q, _P;
    point_eqw_fp_copy(&_Q, Q);
    point_eqw_fp_copy(&_P, P);
    point_eqw_fp_normalise(&_Q);
    point_eqw_fp_normalise(&_P);
    return (fp_is_equal(&_Q.x, &_P.x) && fp_is_equal(&_Q.y, &_P.y));
}

inline void
point_eqw_fp_to_xpoint(xpoint_eqw_fp_t *Px, const point_eqw_fp_t *P)
{
    fp_copy(&Px->x, &P->x);
    fp_copy(&Px->z, &P->z);
}

// Initialize point as identity element (1 : 0)
void xpoint_eqw_fp_init(xpoint_eqw_fp_t *P) { fp_set_one(&(P->x)); fp_set_zero(&(P->z)); }
inline void xpoint_eqw_fp_copy(xpoint_eqw_fp_t *P, const xpoint_eqw_fp_t *Q) { fp_copy(&P->x, &Q->x); fp_copy(&P->z, &Q->z); }

void
xpoint_eqw_fp_normalise(xpoint_eqw_fp_t *P)
{
    fp_inv(&P->z);
    fp_mul(&P->x, &P->x, &P->z);
    fp_set_one(&(P->z));
}

uint32_t xpoint_eqw_fp_is_zero(const xpoint_eqw_fp_t *P) { return fp_is_zero(&P->z); }

uint32_t
xpoint_eqw_fp_is_equal(const xpoint_eqw_fp_t *P, const xpoint_eqw_fp_t *Q)
{
    // Decide whether two points in (X : Z)  coordinates are equal
    // Returns 0xFFFFFFFF (true) if P=Q, 0 (false) otherwise
    fp_t t0, t1;

    // Check if P, Q are the points at infinity
    uint32_t l_zero = xpoint_eqw_fp_is_zero(P);
    uint32_t r_zero = xpoint_eqw_fp_is_zero(Q);

    // Check if PX * QZ == QX * PZ
    fp_mul(&t0, &P->x, &Q->z);
    fp_mul(&t1, &P->z, &Q->x);
    uint32_t lr_equal = fp_is_equal(&t0, &t1);

    // Points are equal if
    // - Both are zero, or
    // - neither are zero and PX * QZ = QX * PZ
    return (l_zero & r_zero) | (~l_zero & ~r_zero * lr_equal);
}

uint32_t
xpoint_eqw_fp_is_two_torsion(const xpoint_eqw_fp_t *P, const curve_mg_fp_t *E)
{
    if (xpoint_eqw_fp_is_zero(P))
        return 0;

    uint32_t x_is_zero, tmp_is_zero;
    fp_t t0, t1, t2;
    fp_add(&t0, &P->x, &P->z);
    fp_sqr(&t0, &t0);
    fp_sub(&t1, &P->x, &P->z);
    fp_sqr(&t1, &t1);
    fp_sub(&t2, &t0, &t1);
    fp_add(&t1, &t0, &t1);
    fp_mul(&t2, &t2, &E->A);
    fp_mul(&t1, &t1, &E->C);
    fp_add(&t1, &t1, &t1);
    fp_add(&t0, &t1, &t2); // 4 (CX^2+CZ^2+AXZ)

    x_is_zero = fp_is_zero(&P->x);
    tmp_is_zero = fp_is_zero(&t0);

    // two torsion if x or x^2 + Ax + 1 is zero
    return x_is_zero | tmp_is_zero;
}

uint32_t
xpoint_eqw_fp_has_zero_coordinate(const xpoint_eqw_fp_t *P)
{
    return fp_is_zero(&P->x) | fp_is_zero(&P->z);
}

// --------------------------------------------------------------------------------
// Curves
// --------------------------------------------------------------------------------

void
curve_mg_fp_init(curve_mg_fp_t *E)
{ // Initialize the curve struct
    // Initialize the constants
    fp_set_zero(&(E->A));
    fp_set_one(&(E->C));

    // Initialize the point (A+2 : 4C)
    xpoint_eqw_fp_init(&(E->A24));

    // Set the bool to be false by default
    E->is_A24_computed_and_normalized = false;
}

void
curve_mg_fp_select_point(xpoint_eqw_fp_t *Q, const xpoint_eqw_fp_t *P1, const xpoint_eqw_fp_t *P2, const digit_t option)
{ // Select points in constant time
  // If option = 0 then Q <- P1, else if option = 0xFF...FF then Q <- P2
    fp_select(&(Q->x), &(P1->x), &(P2->x), option);
    fp_select(&(Q->z), &(P1->z), &(P2->z), option);
}

void
point_eqw_fp_cswap(point_eqw_fp_t *P, point_eqw_fp_t *Q, const digit_t option)
{ // Swap points in constant time
  // If option = 0 then P <- P and Q <- Q, else if option = 0xFF...FF then P <- Q and Q <- P
    fp_cswap(&(P->x), &(Q->x), option);
    fp_cswap(&(P->y), &(Q->y), option);
    fp_cswap(&(P->z), &(Q->z), option);
}

void
xpoint_eqw_fp_cswap(xpoint_eqw_fp_t *P, xpoint_eqw_fp_t *Q, const digit_t option)
{ // Swap points in constant time
  // If option = 0 then P <- P and Q <- Q, else if option = 0xFF...FF then P <- Q and Q <- P
    fp_cswap(&(P->x), &(Q->x), option);
    fp_cswap(&(P->z), &(Q->z), option);
}

void
curve_mg_fp_normalise(curve_mg_fp_t *E)
{
    if (!fp_is_one(&E->C)) {
        fp_inv(&E->C);
        fp_mul(&E->A, &E->A, &E->C);
        fp_set_one(&E->C);
    }
}

void
curve_mg_fp_normalise_A24(curve_mg_fp_t *E)
{
    if (!E->is_A24_computed_and_normalized) {
        curve_mg_fp_AC_to_A24(&E->A24, E);
        xpoint_eqw_fp_normalise(&E->A24);
        E->is_A24_computed_and_normalized = true;
    }
    assert(fp_is_one(&E->A24.z));
}

void
curve_mg_fp_normalise_and_A24(curve_mg_fp_t *E)
{ // Neither the curve or A24 are guaranteed to be normalized.
  // First we normalize (A/C : 1) and conditionally compute
    if (!fp_is_one(&E->C)) {
        curve_mg_fp_normalise(E);
    }

    if (!E->is_A24_computed_and_normalized) {
        // Now compute A24 = ((A + 2) / 4 : 1)
        fp_add_one(&E->A24.x, &E->A);     // re(A24.x) = re(A) + 1
        fp_add_one(&E->A24.x, &E->A24.x); // re(A24.x) = re(A) + 2

        fp_half(&E->A24.x, &E->A24.x); // (A + 2) / 2
        fp_half(&E->A24.x, &E->A24.x); // (A + 2) / 4
        fp_set_one(&E->A24.z);

        E->is_A24_computed_and_normalized = true;
    }
}

int
curve_mg_fp_verify_A(const fp_t *A)
{ // Verify the Montgomery coefficient A is valid (A^2-4 \ne 0)
  // Return 1 if curve is valid, 0 otherwise
    fp_t t;
    fp_set_one(&t);
    fp_add(&t, &t, &t); // t=2
    if (fp_is_equal(A, &t))
        return 0;
    fp_neg(&t, &t); // t=-2
    if (fp_is_equal(A, &t))
        return 0;
    return 1;
}

int
curve_mg_fp_init_from_A(curve_mg_fp_t *E, const fp_t *A)
{ // Initialize the curve from the A coefficient and check it is valid
  // Return 1 if curve is valid, 0 otherwise
    curve_mg_fp_init(E);
    fp_copy(&E->A, A); // Set A
    return curve_mg_fp_verify_A(A);
}

// --------------------------------------------------------------------------------
// Curve Membership
// --------------------------------------------------------------------------------

uint32_t
x_on_curve_fp(fp_t *y_pow2, const fp_t *x, const curve_mg_fp_t *E) {
	fp_t tmp0, tmp1;

    // y_pow2 = x * (x**2 + A * x + 1)
	fp_sqr(&tmp0, x);
	fp_mul(&tmp1, &E->A, x);
	fp_add(&tmp0, &tmp0, &tmp1);
	fp_add_one(&tmp0, &tmp0);
	fp_mul(y_pow2, &tmp0, x);
	return fp_is_square(y_pow2);
}

uint32_t
point_eqw_fp_on_curve(const point_eqw_fp_t *P, const curve_mg_fp_t *E) {
    // Only for normalised points
    assert(fp_is_one(&P->z));

    fp_t y_pow2, y_pow2_actual;
    x_on_curve_fp(&y_pow2, &P->x, E);
    fp_sqr(&y_pow2_actual, &P->y);
	return fp_is_equal(&y_pow2, &y_pow2_actual);
}

// uint32_t
// lift_x_fp(point_eqw_fp_t *P, const fp_t *x, const curve_mg_fp_t *E) {
// 	fp_t tmp0, tmp1;
//     eqw_point_fp_init(P);
//
//     // y_pow2 = x * (x**2 + A * x + 1)
// 	fp_sqr(&tmp0, x);
// 	fp_mul(&tmp1, &E->A, x);
// 	fp_add(&tmp0, &tmp0, &tmp1);
// 	fp_add_one(&tmp0, &tmp0);
// 	fp_mul(&P->y, &tmp0, x);
// 	return fp_is_square(y_pow2);
// }

// --------------------------------------------------------------------------------
// Curve arithmetic
// --------------------------------------------------------------------------------

void
_xpoint_eqw_fp_dbl_E0(
    xpoint_eqw_fp_t *Q,
    const xpoint_eqw_fp_t *P
)
{
    // Doubling of a (X : Z)-point (x(P) = X / Z) on the curve E0 with (A : C) = (0 : 1)
    // Input:  P = (XP : ZP) and Montgomery curve constants (A : C) = (0 : 1)
    // Output: Q = (XQ : ZQ) such that x(2P) = XQ / ZQ
    fp_t t0, t1, t2;

    fp_add(&t0, &P->x, &P->z);
    fp_sqr(&t0, &t0);
    fp_sub(&t1, &P->x, &P->z);
    fp_sqr(&t1, &t1);
    fp_sub(&t2, &t0, &t1);
    fp_add(&t1, &t1, &t1);
    fp_mul(&Q->x, &t0, &t1);
    fp_add(&Q->z, &t1, &t2);
    fp_mul(&Q->z, &Q->z, &t2);
}

void
_xpoint_eqw_fp_dbl_A24(
    xpoint_eqw_fp_t *Q,
    const xpoint_eqw_fp_t *P,
    const xpoint_eqw_fp_t *A24,
    const bool A24_normalized
)
{
    // Doubling of a (X : Z)-point (x(P) = X / Z)
    // Input:  P = (XP : ZP) and Montgomery curve constants A24 = (A + 2C : 4C)
    // Output: Q = (XQ : ZQ) such that x(2P) = XQ / ZQ
    fp_t t0, t1, t2;

    fp_add(&t0, &P->x, &P->z);
    fp_sqr(&t0, &t0);
    fp_sub(&t1, &P->x, &P->z);
    fp_sqr(&t1, &t1);
    fp_sub(&t2, &t0, &t1);
    if (!A24_normalized)
        fp_mul(&t1, &t1, &A24->z);
    fp_mul(&Q->x, &t0, &t1);
    fp_mul(&t0, &t2, &A24->x);
    fp_add(&t0, &t0, &t1);
    fp_mul(&Q->z, &t0, &t2);
}

void
_xpoint_eqw_fp_dbl(
    xpoint_eqw_fp_t *Q,
    const xpoint_eqw_fp_t *P,
    const xpoint_eqw_fp_t *AC
)
{
    // Doubling of a (X : Z)-point (x(P) = X / Z)
    // Input:  P = (XP : ZP) and Montgomery curve constants (A : C)
    // Output: Q = (XQ : ZQ) such that x(2P) = XQ / ZQ
    fp_t t0, t1, t2, t3;

    fp_add(&t0, &P->x, &P->z);
    fp_sqr(&t0, &t0);
    fp_sub(&t1, &P->x, &P->z);
    fp_sqr(&t1, &t1);
    fp_sub(&t2, &t0, &t1);
    fp_add(&t3, &AC->z, &AC->z);
    fp_mul(&t1, &t1, &t3);
    fp_add(&t1, &t1, &t1);
    fp_mul(&Q->x, &t0, &t1);
    fp_add(&t0, &t3, &AC->x);
    fp_mul(&t0, &t0, &t2);
    fp_add(&t0, &t0, &t1);
    fp_mul(&Q->z, &t0, &t2);
}

void
xpoint_eqw_fp_add(
    xpoint_eqw_fp_t *R,
    const xpoint_eqw_fp_t *P,
    const xpoint_eqw_fp_t *Q,
    const xpoint_eqw_fp_t *PmQ
) {
    // Differential addition of (X : Z)-points (x(P) = X / Z)
    // Input:  P = (XP : ZP), Q = (XQ : ZQ), and PmQ = P - Q = (XPmQ : ZPmQ)
    // Output: R = = (XR : ZR) such that x(P + Q) = XR / ZR
    fp_t t0, t1, t2, t3;

    fp_add(&t0, &P->x, &P->z);
    fp_sub(&t1, &P->x, &P->z);
    fp_add(&t2, &Q->x, &Q->z);
    fp_sub(&t3, &Q->x, &Q->z);
    fp_mul(&t0, &t0, &t3);
    fp_mul(&t1, &t1, &t2);
    fp_add(&t2, &t0, &t1);
    fp_sub(&t3, &t0, &t1);
    fp_sqr(&t2, &t2);
    fp_sqr(&t3, &t3);
    fp_mul(&t2, &PmQ->z, &t2);
    fp_mul(&R->z, &PmQ->x, &t3);
    fp_copy(&R->x, &t2);
}

void
_xpoint_eqw_fp_dbladd(xpoint_eqw_fp_t *R,
        xpoint_eqw_fp_t *S,
        const xpoint_eqw_fp_t *P,
        const xpoint_eqw_fp_t *Q,
        const xpoint_eqw_fp_t *PQ,
        const xpoint_eqw_fp_t *A24,
        const bool A24_normalized)
{ // Simultaneous doubling and differential addition.
  // Input:  projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, the difference
  //         PQ=P-Q=(XPQ:ZPQ), and the Montgomery curve constants A24 = (A+2C:4C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery points R <- 2*P = (XR:ZR) such that x(2P)=XR/ZR, and S <- P+Q = (XS:ZS) such that =
  //         x(Q+P)=XS/ZS.
    fp_t t0, t1, t2;

    fp_add(&t0, &P->x, &P->z);
    fp_sub(&t1, &P->x, &P->z);
    fp_sqr(&R->x, &t0);
    fp_sub(&t2, &Q->x, &Q->z);
    fp_add(&S->x, &Q->x, &Q->z);
    fp_mul(&t0, &t0, &t2);
    fp_sqr(&R->z, &t1);
    fp_mul(&t1, &t1, &S->x);
    fp_sub(&t2, &R->x, &R->z);
    if (!A24_normalized)
        fp_mul(&R->z, &R->z, &A24->z);
    fp_mul(&R->x, &R->x, &R->z);
    fp_mul(&S->x, &A24->x, &t2);
    fp_sub(&S->z, &t0, &t1);
    fp_add(&R->z, &R->z, &S->x);
    fp_add(&S->x, &t0, &t1);
    fp_mul(&R->z, &R->z, &t2);
    fp_sqr(&S->z, &S->z);
    fp_sqr(&S->x, &S->x);
    fp_mul(&S->z, &S->z, &PQ->x);
    fp_mul(&S->x, &S->x, &PQ->z);
}

void
_xpoint_eqw_fp_mul(
    xpoint_eqw_fp_t *Q,
    xpoint_eqw_fp_t *S,
    const xpoint_eqw_fp_t *P,
    const digit_t *k,
    const int kbits,
    const curve_mg_fp_t *curve
)
{ // The Montgomery ladder
  // Input: projective Montgomery point P=(XP:ZP) such that xP=XP/ZP, a scalar k of bitlength kbits, and
  //        the Montgomery curve constants (A:C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery points Q <- k*P = (XQ:ZQ) such that x(k*P)=XQ/ZQ,
  //         projective Montgomery points S <- (k+1)*P = (XS:ZS) such that x((k+1)*P)=XS/ZS
  // Ref: Algorithm 4, https://eprint.iacr.org/2017/212 (The variant that also returns x1)
    xpoint_eqw_fp_t R0, R1, A24;
    digit_t mask;
    unsigned int bit, prevbit = 0, swap;

    if (!curve->is_A24_computed_and_normalized) {
        // Computation of A24=(A+2C:4C)
        fp_add(&A24.x, &curve->C, &curve->C);
        fp_add(&A24.z, &A24.x, &A24.x);
        fp_add(&A24.x, &A24.x, &curve->A);
    } else {
        fp_copy(&A24.x, &curve->A24.x);
        fp_copy(&A24.z, &curve->A24.z);
        // Assert A24 has been normalised
        assert(fp_is_one(&A24.z));
    }

    // R0 <- (1:0), R1 <- P
    xpoint_eqw_fp_init(&R0);
    fp_copy(&R1.x, &P->x);
    fp_copy(&R1.z, &P->z);

    // Main loop
    for (int i = kbits - 1; i >= 0; i--) {
        bit = (k[i >> LOG2RADIX] >> (i & (RADIX - 1))) & 1;
        swap = bit ^ prevbit;
        prevbit = bit;
        mask = 0 - (digit_t)swap;

        xpoint_eqw_fp_cswap(&R0, &R1, mask);
        _xpoint_eqw_fp_dbladd(&R0, &R1, &R0, &R1, P, &A24, true);
    }

    swap = 0 ^ prevbit;
    mask = 0 - (digit_t)swap;
    xpoint_eqw_fp_cswap(&R0, &R1, mask);

    fp_copy(&Q->x, &R0.x);
    fp_copy(&Q->z, &R0.z);
    fp_copy(&S->x, &R1.x);
    fp_copy(&S->z, &R1.z);
}

void
_xpoint_eqw_fp_dbl_iter(
    xpoint_eqw_fp_t *Q,
    xpoint_eqw_fp_t *S,
    const xpoint_eqw_fp_t *P,
    const int k,
    const curve_mg_fp_t *curve
)
{ // The Montgomery ladder
  // Input: projective Montgomery point P=(XP:ZP) such that xP=XP/ZP, a scalar k of bitlength kbits, and
  //        the Montgomery curve constants (A:C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery points Q <- k*P = (XQ:ZQ) such that x(k*P)=XQ/ZQ,
  //         projective Montgomery points S <- (k+1)*P = (XS:ZS) such that x((k+1)*P)=XS/ZS
  // Ref: Algorithm 4, https://eprint.iacr.org/2017/212 (The variant that also returns x1)
    xpoint_eqw_fp_t R0, R1, A24;
    digit_t mask;
    unsigned int bit, prevbit = 0, swap;

    if (!curve->is_A24_computed_and_normalized) {
        // Computation of A24=(A+2C:4C)
        fp_add(&A24.x, &curve->C, &curve->C);
        fp_add(&A24.z, &A24.x, &A24.x);
        fp_add(&A24.x, &A24.x, &curve->A);
    } else {
        fp_copy(&A24.x, &curve->A24.x);
        fp_copy(&A24.z, &curve->A24.z);
        // Assert A24 has been normalised
        assert(fp_is_one(&A24.z));
    }

    // R0 <- (1:0), R1 <- P
    xpoint_eqw_fp_init(&R0);
    fp_copy(&R1.x, &P->x);
    fp_copy(&R1.z, &P->z);

    // Main loop
    for (int i = 0; i < k + 1; i++) {
        bit = (i == 0) ? 1 : 0;
        swap = bit ^ prevbit;
        prevbit = bit;
        mask = 0 - (digit_t)swap;

        xpoint_eqw_fp_cswap(&R0, &R1, mask);
        _xpoint_eqw_fp_dbladd(&R0, &R1, &R0, &R1, P, &A24, true);
    }

    swap = 0 ^ prevbit;
    mask = 0 - (digit_t)swap;
    xpoint_eqw_fp_cswap(&R0, &R1, mask);

    fp_copy(&Q->x, &R0.x);
    fp_copy(&Q->z, &R0.z);
    fp_copy(&S->x, &R1.x);
    fp_copy(&S->z, &R1.z);
}

int
_xpoint_eqw_fp_dblMUL(xpoint_eqw_fp_t *S,
        const xpoint_eqw_fp_t *P,
        const digit_t *k,
        const xpoint_eqw_fp_t *Q,
        const digit_t *l,
        const xpoint_eqw_fp_t *PQ,
        const int kbits,
        const curve_mg_fp_t *curve)
{ // The Montgomery biladder
  // Input:  projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, scalars k and l of
  //         bitlength kbits, the difference PQ=P-Q=(XPQ:ZPQ), and the Montgomery curve constants (A:C).
  // Output: projective Montgomery point S <- k*P + l*Q = (XS:ZS) such that x(k*P + l*Q)=XS/ZS.

    int i, A_is_zero;
    digit_t evens, mevens, bitk0, bitl0, maskk, maskl, temp, bs1_ip1, bs2_ip1, bs1_i, bs2_i, h;
    digit_t sigma[2] = { 0 }, pre_sigma = 0;
    digit_t k_t[NWORDS_ORDER], l_t[NWORDS_ORDER], one[NWORDS_ORDER] = { 0 }, r[2 * BITS] = { 0 };
    xpoint_eqw_fp_t DIFF1a, DIFF1b, DIFF2a, DIFF2b, R[3] = { 0 }, T[3];

    // differential additions formulas are invalid in this case
    if (xpoint_eqw_fp_has_zero_coordinate(P) | xpoint_eqw_fp_has_zero_coordinate(Q) | xpoint_eqw_fp_has_zero_coordinate(PQ))
        return 0;

    // Derive sigma according to parity
    bitk0 = (k[0] & 1);
    bitl0 = (l[0] & 1);
    maskk = 0 - bitk0; // Parity masks: 0 if even, otherwise 1...1
    maskl = 0 - bitl0;
    sigma[0] = (bitk0 ^ 1);
    sigma[1] = (bitl0 ^ 1);
    evens = sigma[0] + sigma[1]; // Count number of even scalars
    mevens = 0 - (evens & 1);    // Mask mevens <- 0 if # even of scalars = 0 or 2, otherwise mevens = 1...1

    // If k and l are both even or both odd, pick sigma = (0,1)
    sigma[0] = (sigma[0] & mevens);
    sigma[1] = (sigma[1] & mevens) | (1 & ~mevens);

    // Convert even scalars to odd
    one[0] = 1;
    mp_sub(k_t, k, one, NWORDS_ORDER);
    mp_sub(l_t, l, one, NWORDS_ORDER);
    select_ct(k_t, k_t, k, maskk, NWORDS_ORDER);
    select_ct(l_t, l_t, l, maskl, NWORDS_ORDER);

    // Scalar recoding
    for (i = 0; i < kbits; i++) {
        // If sigma[0] = 1 swap k_t and l_t
        maskk = 0 - (sigma[0] ^ pre_sigma);
        swap_ct(k_t, l_t, maskk, NWORDS_ORDER);

        if (i == kbits - 1) {
            bs1_ip1 = 0;
            bs2_ip1 = 0;
        } else {
            bs1_ip1 = mp_shiftr(k_t, 1, NWORDS_ORDER);
            bs2_ip1 = mp_shiftr(l_t, 1, NWORDS_ORDER);
        }
        bs1_i = k_t[0] & 1;
        bs2_i = l_t[0] & 1;

        r[2 * i] = bs1_i ^ bs1_ip1;
        r[2 * i + 1] = bs2_i ^ bs2_ip1;

        // Revert sigma if second bit, r_(2i+1), is 1
        pre_sigma = sigma[0];
        maskk = 0 - r[2 * i + 1];
        select_ct(&temp, &sigma[0], &sigma[1], maskk, 1);
        select_ct(&sigma[1], &sigma[1], &sigma[0], maskk, 1);
        sigma[0] = temp;
    }

    // Point initialization
    xpoint_eqw_fp_init(&R[0]);
    maskk = 0 - sigma[0];
    curve_mg_fp_select_point(&R[1], P, Q, maskk);
    curve_mg_fp_select_point(&R[2], Q, P, maskk);

    fp_copy(&DIFF1a.x, &R[1].x);
    fp_copy(&DIFF1a.z, &R[1].z);
    fp_copy(&DIFF1b.x, &R[2].x);
    fp_copy(&DIFF1b.z, &R[2].z);

    // Initialize DIFF2a <- P+Q, DIFF2b <- P-Q
    xpoint_eqw_fp_add(&R[2], &R[1], &R[2], PQ);
    if (xpoint_eqw_fp_has_zero_coordinate(&R[2]))
        return 0; // non valid formulas

    fp_copy(&DIFF2a.x, &R[2].x);
    fp_copy(&DIFF2a.z, &R[2].z);
    fp_copy(&DIFF2b.x, &PQ->x);
    fp_copy(&DIFF2b.z, &PQ->z);

    A_is_zero = fp_is_zero(&curve->A);

    // Main loop
    for (i = kbits - 1; i >= 0; i--) {
        h = r[2 * i] + r[2 * i + 1]; // in {0, 1, 2}
        maskk = 0 - (h & 1);
        curve_mg_fp_select_point(&T[0], &R[0], &R[1], maskk);
        maskk = 0 - (h >> 1);
        curve_mg_fp_select_point(&T[0], &T[0], &R[2], maskk);
        if (A_is_zero) {
            _xpoint_eqw_fp_dbl_E0(&T[0], &T[0]);
        } else {
            assert(fp_is_one(&curve->A24.z));
            _xpoint_eqw_fp_dbl_A24(&T[0], &T[0], &curve->A24, true);
        }

        maskk = 0 - r[2 * i + 1]; // in {0, 1}
        curve_mg_fp_select_point(&T[1], &R[0], &R[1], maskk);
        curve_mg_fp_select_point(&T[2], &R[1], &R[2], maskk);

        xpoint_eqw_fp_cswap(&DIFF1a, &DIFF1b, maskk);
        xpoint_eqw_fp_add(&T[1], &T[1], &T[2], &DIFF1a);
        xpoint_eqw_fp_add(&T[2], &R[0], &R[2], &DIFF2a);

        // If hw (mod 2) = 1 then swap DIFF2a and DIFF2b
        maskk = 0 - (h & 1);
        xpoint_eqw_fp_cswap(&DIFF2a, &DIFF2b, maskk);

        // R <- T
        xpoint_eqw_fp_copy(&R[0], &T[0]);
        xpoint_eqw_fp_copy(&R[1], &T[1]);
        xpoint_eqw_fp_copy(&R[2], &T[2]);
    }

    // Output R[evens]
    curve_mg_fp_select_point(S, &R[0], &R[1], mevens);

    maskk = 0 - (bitk0 & bitl0);
    curve_mg_fp_select_point(S, S, &R[2], maskk);
    return 1;
}

// WRAPPERS to export

void
xpoint_eqw_fp_dbl(xpoint_eqw_fp_t *res, const xpoint_eqw_fp_t *P, const curve_mg_fp_t *curve)
{
    // If A24 = ((A+2)/4 : 1) we save multiplications
    if (curve->is_A24_computed_and_normalized) {
        assert(fp_is_one(&curve->A24.z));
        _xpoint_eqw_fp_dbl_A24(res, P, &curve->A24, true);
    } else {
        // Otherwise we compute A24 on the fly for doubling
        _xpoint_eqw_fp_dbl(res, P, (const xpoint_eqw_fp_t *)curve);
    }
}

void
xpoint_eqw_fp_dbl_iter(xpoint_eqw_fp_t *res, int n, const xpoint_eqw_fp_t *P, curve_mg_fp_t *curve)
{
    if (n == 0) {
        xpoint_eqw_fp_copy(res, P);
        return;
    }

    // When the chain is long enough, we should normalise A24
    if (n > 50) {
        curve_mg_fp_normalise_A24(curve);
    }

    // When A24 is normalized we can save some multiplications
    if (curve->is_A24_computed_and_normalized) {
        assert(fp_is_one(&curve->A24.z));
        _xpoint_eqw_fp_dbl_A24(res, P, &curve->A24, true);
        for (int i = 0; i < n - 1; i++) {
            assert(fp_is_one(&curve->A24.z));
            _xpoint_eqw_fp_dbl_A24(res, res, &curve->A24, true);
        }
    } else {
        // Otherwise we do normal doubling
        _xpoint_eqw_fp_dbl(res, P, (const xpoint_eqw_fp_t *)curve);
        for (int i = 0; i < n - 1; i++) {
            _xpoint_eqw_fp_dbl(res, res, (const xpoint_eqw_fp_t *)curve);
        }
    }
}

void
xpoint_eqw_fp_mul(xpoint_eqw_fp_t *res, xpoint_eqw_fp_t *resp1, const digit_t *scalar, const int kbits, const xpoint_eqw_fp_t *P, curve_mg_fp_t *curve)
{
    // For large scalars it's worth normalising anyway
    if (kbits > 50) {
        curve_mg_fp_normalise_A24(curve);
    }

    // When A24 is computed and normalized we save some fp multiplications
    _xpoint_eqw_fp_mul(res, resp1, P, scalar, kbits, curve);
}

void
xpoint_eqw_fp_mul_dbl_iter(
    xpoint_eqw_fp_t *res,
    xpoint_eqw_fp_t *resp1,
    const int k,
    const xpoint_eqw_fp_t *P,
    curve_mg_fp_t *curve
) {
    // For large scalars it's worth normalising anyway
    if (k > 50)
        curve_mg_fp_normalise_A24(curve);

    // When A24 is computed and normalized we save some fp multiplications
    _xpoint_eqw_fp_dbl_iter(res, resp1, P, k, curve);
}

void
sw_curve_normalise_fp(curve_sw_fp_t *E2, const curve_sw_fp_t *E1) {
    fp_t C_inv, C_inv_pow2;
    fp_copy(&C_inv, &E1->C);
    fp_inv(&C_inv);
    fp_sqr(&C_inv_pow2, &C_inv);
    fp_mul(&E2->A, &E1->A, &C_inv_pow2);
    fp_mul(&E2->B, &E1->B, &C_inv_pow2);
    fp_mul(&E2->B, &E2->B, &C_inv);
    fp_set_one(&E2->C);
}

void
mg_to_sw_fp(
        curve_sw_fp_t *E_sw,
        xpoint_eqw_fp_t *xpts_sw,
        point_eqw_fp_t *pts_sw,
        const curve_mg_fp_t *E_m,
        const xpoint_eqw_fp_t *xpts_m,
        const uint32_t len_xpts,
        const point_eqw_fp_t *pts_m,
        const uint32_t len_pts
) {
    // Computes isomorphism from Montgomery form to short Weierstrass from
    // i.e. computes isomorphism
    //   phi: E_m: By^2 = x(x^2 + Ax + 1) -> E_w: y^2 = x^3 + ax + b
    // and maps both x-points and points through
    // Note: These points may be y-twisted, i.e. such that (x, eye * y) lies on E
    // Ref: https://eprint.iacr.org/2017/212 Sec 2.4 (Case B = 1)
    // Cost: Curve: 10a/s + 2M, Per x-point: 1a/s + 2M, Per point: 1a/s + 3M

    fp_t A, C;
    fp_copy(&A, &E_m->A);
    fp_copy(&C, &E_m->C);

    fp_t A_pow2, C_pow2, C_pow2_mul9;
    fp_sqr(&A_pow2, &A);

    fp_sqr(&C_pow2, &C);
    // fp_mul_small(&C_pow2_mul9, &C_pow2, 9);
        fp_add(&C_pow2_mul9, &C_pow2, &C_pow2);
        fp_add(&C_pow2_mul9, &C_pow2_mul9, &C_pow2_mul9);
        fp_add(&C_pow2_mul9, &C_pow2_mul9, &C_pow2_mul9);
        fp_add(&C_pow2_mul9, &C_pow2_mul9, &C_pow2);

    // A_new = 9 * C^2 - 3 * A^2
    fp_sub(&E_sw->A, &C_pow2_mul9, &A_pow2);
    fp_sub(&E_sw->A, &E_sw->A, &A_pow2);
    fp_sub(&E_sw->A, &E_sw->A, &A_pow2);

    // B_new = A * (2 * A^2 - 9 * C^2)
    fp_add(&E_sw->B, &A_pow2, &A_pow2);
    fp_sub(&E_sw->B, &E_sw->B, &C_pow2_mul9);
    fp_mul(&E_sw->B, &E_sw->B, &A);

    // C_new = 3 * C
    // fp_mul_small(&E_sw->C, &C, 3);
        fp_add(&E_sw->C, &C, &C);
        fp_add(&E_sw->C, &E_sw->C, &C);

    fp_t *const C_mul3 = &E_sw->C;

    fp_t tmp;

    for (uint32_t i = 0; i < len_xpts; i++) {
        // (Px_new, Pz_new) = (3 * C * Px + A * Pz, 3 * C * Pz)
        fp_mul(&(xpts_sw + i)->x, C_mul3, &(xpts_m + i)->x);
        fp_mul(&tmp, &A, &(xpts_m + i)->z);
        fp_add(&(xpts_sw + i)->x, &(xpts_sw + i)->x, &tmp);
        fp_mul(&(xpts_sw + i)->z, C_mul3, &(xpts_m + i)->z);
    }

    for (uint32_t i = 0; i < len_pts; i++) {
        // (Px_new, Py_new, Pz_new) = (3 * C * Px + A * Pz, 3 * C * Py, 3 * C * Pz)
        fp_mul(&(pts_sw + i)->x, C_mul3, &(pts_m + i)->x);
        fp_mul(&tmp, &A, &(pts_m + i)->z);
        fp_add(&(pts_sw + i)->x, &(pts_sw + i)->x, &tmp);
        fp_mul(&(pts_sw + i)->y, C_mul3, &(pts_m + i)->y);
        fp_mul(&(pts_sw + i)->z, C_mul3, &(pts_m + i)->z);
    }
}

void
sw_to_mg_fp(
        curve_mg_fp_t *E_m,
        xpoint_eqw_fp_t *xpts_m,
        point_eqw_fp_t *pts_m,
        const curve_sw_fp_t *E_sw,
        const xpoint_eqw_fp_t *xT8,
        const xpoint_eqw_fp_t *xpts_sw,
        const uint32_t len_xpts,
        const point_eqw_fp_t *pts_sw,
        const uint32_t len_pts
) {
    xpoint_eqw_fp_t alpha_pt;

    fp_t C_pow2, C_pow3;
    fp_sqr(&C_pow2, &E_sw->C);
    fp_mul(&C_pow3, &C_pow2, &E_sw->C);

    sw_xdouble_fp(&alpha_pt, E_sw, &C_pow2, &C_pow3, xT8);
    sw_xdouble_fp(&alpha_pt, E_sw, &C_pow2, &C_pow3, &alpha_pt);

    fp_t a, alpha, beta_pow2, beta, beta_sqrt;

    // Normalise alpha and E.a
    fp_div(&alpha, &alpha_pt.x, &alpha_pt.z);
    // a = E.a / E.c^2
    // (We won't need normalised E.b, so we only do E.a manually)
    fp_sqr(&a, &E_sw->C);
    fp_div(&a, &E_sw->A, &a);

    #ifndef NDEBUG
        // Verify that alpha^3 + (E.A / E.C^2) * alpha + (E.B / E.C3) == 0
        // i.e. alpha is x-coordinate of 2-torsion point
        fp_t tmp1, tmp2, tmp3;
        fp_sqr(&tmp1, &alpha);
        fp_mul(&tmp1, &tmp1, &alpha);

        fp_div(&tmp2, &E_sw->A, &C_pow2);
        fp_mul(&tmp2, &alpha, &tmp2);

        fp_div(&tmp3, &E_sw->B, &C_pow3);
        fp_add(&tmp1, &tmp1, &tmp2);
        fp_add(&tmp1, &tmp1, &tmp3);
        assert(fp_is_zero(&tmp1));
    #endif

    // beta_pow2 = 3 * alpha^2 + a
    fp_sqr(&beta_pow2, &alpha);
    // fp_mul_small(&beta_pow2, &beta_pow2, 3);
        fp_add(&beta, &beta_pow2, &beta_pow2);
        fp_add(&beta_pow2, &beta_pow2, &beta);
    fp_add(&beta_pow2, &beta_pow2, &a);

    fp_copy(&beta, &beta_pow2);

    fp_copy(&beta, &beta_pow2);
    fp_sqrt(&beta);

    if (!fp_is_square(&beta))
        fp_neg(&beta, &beta);
    assert(fp_is_square(&beta));

    fp_copy(&beta_sqrt, &beta);
    fp_sqrt(&beta_sqrt);

    // fp_mul_small(&E_m->A, &alpha, 3);
        fp_add(&E_m->C, &alpha, &alpha);
        fp_add(&E_m->A, &E_m->C, &alpha);
    fp_copy(&E_m->C, &beta);

    for (uint32_t i = 0; i < len_xpts; i++) {
        // x, z = x - alpha * z, z * beta
        fp_mul(&(xpts_m + i)->x, &alpha, &(xpts_sw + i)->z);
        fp_sub(&(xpts_m + i)->x, &(xpts_sw + i)->x, &(xpts_m + i)->x);
        fp_mul(&(xpts_m + i)->z, &beta, &(xpts_sw + i)->z);
    }

    for (uint32_t i = 0; i < len_pts; i++) {
        // x, y, z = (x - alpha * z) * beta, y * beta_sqrt, z * beta_pow2
        fp_mul(&(pts_m + i)->x, &alpha, &(pts_sw + i)->z);
        fp_sub(&(pts_m + i)->x, &(pts_sw + i)->x, &(pts_m + i)->x);
        fp_mul(&(pts_m + i)->x, &(pts_m + i)->x, &beta);
        fp_mul(&(pts_m + i)->y, &beta_sqrt, &(pts_sw + i)->y);
        fp_mul(&(pts_m + i)->z, &beta_pow2, &(pts_sw + i)->z);
    }
}


void
point_eqw_fp_mul(point_eqw_fp_t *kP, digit_t const *k, uint32_t const kbits, const point_eqw_fp_t *P, curve_mg_fp_t *E)
{
    // Algorithm 6, https://eprint.iacr.org/2017/212
    // WARNING: Absolutely not constant time!

    point_eqw_fp_t P_copy;
    point_eqw_fp_copy(&P_copy, P);

    // Ugly for now before refactoring
    xpoint_eqw_fp_t Px, kPx, kp1Px;
    fp_copy(&Px.x, &P->x);
    fp_copy(&Px.z, &P->z);
    xpoint_eqw_fp_mul(&kPx, &kp1Px, k, kbits, &Px, E);
    xpoint_eqw_fp_recover_y(kP, &P_copy, &kPx, &kp1Px, E);
}

void
point_eqw_fp_dbl_iter(point_eqw_fp_t *kP, uint32_t const k, const point_eqw_fp_t *P, curve_mg_fp_t *E)
{
    // Algorithm 6, https://eprint.iacr.org/2017/212
    // WARNING: Absolutely not constant time!

    point_eqw_fp_t P_copy;
    point_eqw_fp_copy(&P_copy, P);

    xpoint_eqw_fp_t Px, kPx, kp1Px;
    point_eqw_fp_to_xpoint(&Px, P);

    xpoint_eqw_fp_mul_dbl_iter(&kPx, &kp1Px, k, &Px, E);
    xpoint_eqw_fp_recover_y(kP, &P_copy, &kPx, &kp1Px, E);

}

void
point_eqw_fp_neg(point_eqw_fp_t *Q, point_eqw_fp_t *P) {
    fp_copy(&Q->x, &P->x);
    fp_neg(&Q->y, &P->y);
    fp_copy(&Q->z, &P->z);
}
