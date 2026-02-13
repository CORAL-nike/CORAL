#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "ec.h"
#include "mp.h"

// --------------------------------------------------------------------------------
// Points
// --------------------------------------------------------------------------------

// Initialize point as identity element (0 : 1 : 0)
void point_eqw_fp2_init(point_eqw_fp2_t *P) { fp2_set_zero(&(P->x)); fp2_set_one(&(P->y)); fp2_set_zero(&(P->z)); }
inline void point_eqw_fp2_copy(point_eqw_fp2_t *P, const point_eqw_fp2_t *Q) { fp2_copy(&P->x, &Q->x); fp2_copy(&P->y, &Q->y); fp2_copy(&P->z, &Q->z); }

void
point_eqw_fp2_normalise(point_eqw_fp2_t *P)
{
    if (!fp2_is_one(&P->z)) {
        fp2_inv(&P->z);
        fp2_mul(&P->x, &P->x, &P->z);
        fp2_mul(&P->y, &P->y, &P->z);
        fp2_set_one(&(P->z));
    }
}

uint32_t
eqw_point_is_equal_fp2(const point_eqw_fp2_t *Q, const point_eqw_fp2_t *P)
{
    point_eqw_fp2_t _Q, _P;
    point_eqw_fp2_copy(&_Q, Q);
    point_eqw_fp2_copy(&_P, P);
    point_eqw_fp2_normalise(&_Q);
    point_eqw_fp2_normalise(&_P);
    return (fp2_is_equal(&_Q.x, &_P.x) && fp2_is_equal(&_Q.y, &_P.y));
}

void
point_eqw_fp2_to_xpoint(xpoint_eqw_fp2_t *Px, const point_eqw_fp2_t *P)
{
    fp2_copy(&Px->x, &P->x);
    fp2_copy(&Px->z, &P->z);
}

// Initialize point as identity element (1 : 0)
void xpoint_eqw_fp2_init(xpoint_eqw_fp2_t *P) { fp2_set_one(&(P->x)); fp2_set_zero(&(P->z)); }
inline void xpoint_eqw_fp2_copy(xpoint_eqw_fp2_t *P, const xpoint_eqw_fp2_t *Q) { fp2_copy(&P->x, &Q->x); fp2_copy(&P->z, &Q->z); }

void
xpoint_eqw_fp2_normalise(xpoint_eqw_fp2_t *P)
{
    fp2_inv(&P->z);
    fp2_mul(&P->x, &P->x, &P->z);
    fp2_set_one(&(P->z));
}

uint32_t xpoint_eqw_fp2_is_zero(const xpoint_eqw_fp2_t *P) { return fp2_is_zero(&P->z); }

uint32_t
xpoint_eqw_fp2_is_equal(const xpoint_eqw_fp2_t *P, const xpoint_eqw_fp2_t *Q)
{
    // Decide whether two points in (X : Z)  coordinates are equal
    // Returns 0xFFFFFFFF (true) if P=Q, 0 (false) otherwise
    fp2_t t0, t1;

    // Check if P, Q are the points at infinity
    uint32_t l_zero = xpoint_eqw_fp2_is_zero(P);
    uint32_t r_zero = xpoint_eqw_fp2_is_zero(Q);

    // Check if PX * QZ == QX * PZ
    fp2_mul(&t0, &P->x, &Q->z);
    fp2_mul(&t1, &P->z, &Q->x);
    uint32_t lr_equal = fp2_is_equal(&t0, &t1);

    // Points are equal if
    // - Both are zero, or
    // - neither are zero and PX * QZ = QX * PZ
    return (l_zero & r_zero) | (~l_zero & ~r_zero * lr_equal);
}

uint32_t
xpoint_eqw_fp2_is_two_torsion(const xpoint_eqw_fp2_t *P, const curve_mg_fp2_t *E)
{
    if (xpoint_eqw_fp2_is_zero(P))
        return 0;

    uint32_t x_is_zero, tmp_is_zero;
    fp2_t t0, t1, t2;
    fp2_add(&t0, &P->x, &P->z);
    fp2_sqr(&t0, &t0);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&t1, &t1);
    fp2_sub(&t2, &t0, &t1);
    fp2_add(&t1, &t0, &t1);
    fp2_mul(&t2, &t2, &E->A);
    fp2_mul(&t1, &t1, &E->C);
    fp2_add(&t1, &t1, &t1);
    fp2_add(&t0, &t1, &t2); // 4 (CX^2+CZ^2+AXZ)

    x_is_zero = fp2_is_zero(&P->x);
    tmp_is_zero = fp2_is_zero(&t0);

    // two torsion if x or x^2 + Ax + 1 is zero
    return x_is_zero | tmp_is_zero;
}

uint32_t
xpoint_eqw_fp2_has_zero_coordinate(const xpoint_eqw_fp2_t *P)
{
    return fp2_is_zero(&P->x) | fp2_is_zero(&P->z);
}

// --------------------------------------------------------------------------------
// Curves
// --------------------------------------------------------------------------------

void
curve_mg_fp2_init(curve_mg_fp2_t *E)
{ // Initialize the curve struct
    // Initialize the constants
    fp2_set_zero(&(E->A));
    fp2_set_one(&(E->C));

    // Initialize the point (A+2 : 4C)
    xpoint_eqw_fp2_init(&(E->A24));

    // Set the bool to be false by default
    E->is_A24_computed_and_normalized = false;
}

void
curve_mg_fp2_select_point(xpoint_eqw_fp2_t *Q, const xpoint_eqw_fp2_t *P1, const xpoint_eqw_fp2_t *P2, const digit_t option)
{ // Select points in constant time
  // If option = 0 then Q <- P1, else if option = 0xFF...FF then Q <- P2
    fp2_select(&(Q->x), &(P1->x), &(P2->x), option);
    fp2_select(&(Q->z), &(P1->z), &(P2->z), option);
}

void
point_eqw_fp2_cswap(point_eqw_fp2_t *P, point_eqw_fp2_t *Q, const digit_t option)
{ // Swap points in constant time
  // If option = 0 then P <- P and Q <- Q, else if option = 0xFF...FF then P <- Q and Q <- P
    fp2_cswap(&(P->x), &(Q->x), option);
    fp2_cswap(&(P->y), &(Q->y), option);
    fp2_cswap(&(P->z), &(Q->z), option);
}

void
xpoint_eqw_fp2_cswap(xpoint_eqw_fp2_t *P, xpoint_eqw_fp2_t *Q, const digit_t option)
{ // Swap points in constant time
  // If option = 0 then P <- P and Q <- Q, else if option = 0xFF...FF then P <- Q and Q <- P
    fp2_cswap(&(P->x), &(Q->x), option);
    fp2_cswap(&(P->z), &(Q->z), option);
}

void
curve_mg_fp2_normalise(curve_mg_fp2_t *E)
{
    if (!fp2_is_one(&E->C)) {
        fp2_inv(&E->C);
        fp2_mul(&E->A, &E->A, &E->C);
        fp2_set_one(&E->C);
    }
}

void
curve_mg_fp2_normalise_A24(curve_mg_fp2_t *E)
{
    if (!E->is_A24_computed_and_normalized) {
        curve_mg_fp2_AC_to_A24(&E->A24, E);
        xpoint_eqw_fp2_normalise(&E->A24);
        E->is_A24_computed_and_normalized = true;
    }
    assert(fp2_is_one(&E->A24.z));
}

void
curve_mg_fp2_normalise_and_A24(curve_mg_fp2_t *E)
{ // Neither the curve or A24 are guaranteed to be normalized.
  // First we normalize (A/C : 1) and conditionally compute
    if (!fp2_is_one(&E->C)) {
        curve_mg_fp2_normalise(E);
    }

    if (!E->is_A24_computed_and_normalized) {
        // Now compute A24 = ((A + 2) / 4 : 1)
        fp2_add_one(&E->A24.x, &E->A);     // re(A24.x) = re(A) + 1
        fp2_add_one(&E->A24.x, &E->A24.x); // re(A24.x) = re(A) + 2
        fp_copy(&E->A24.x.im, &E->A.im);   // im(A24.x) = im(A)

        fp2_half(&E->A24.x, &E->A24.x); // (A + 2) / 2
        fp2_half(&E->A24.x, &E->A24.x); // (A + 2) / 4
        fp2_set_one(&E->A24.z);

        E->is_A24_computed_and_normalized = true;
    }
}

int
curve_mg_fp2_verify_A(const fp2_t *A)
{ // Verify the Montgomery coefficient A is valid (A^2-4 \ne 0)
  // Return 1 if curve is valid, 0 otherwise
    fp2_t t;
    fp2_set_one(&t);
    fp_add(&t.re, &t.re, &t.re); // t=2
    if (fp2_is_equal(A, &t))
        return 0;
    fp_neg(&t.re, &t.re); // t=-2
    if (fp2_is_equal(A, &t))
        return 0;
    return 1;
}

int
curve_mg_fp2_init_from_A(curve_mg_fp2_t *E, const fp2_t *A)
{ // Initialize the curve from the A coefficient and check it is valid
  // Return 1 if curve is valid, 0 otherwise
    curve_mg_fp2_init(E);
    fp2_copy(&E->A, A); // Set A
    return curve_mg_fp2_verify_A(A);
}

// --------------------------------------------------------------------------------
// Curve arithmetic
// --------------------------------------------------------------------------------

void
_xpoint_eqw_fp2_dbl_E0(
    xpoint_eqw_fp2_t *Q,
    const xpoint_eqw_fp2_t *P
)
{
    // Doubling of a (X : Z)-point (x(P) = X / Z) on the curve E0 with (A : C) = (0 : 1)
    // Input:  P = (XP : ZP) and Montgomery curve constants (A : C) = (0 : 1)
    // Output: Q = (XQ : ZQ) such that x(2P) = XQ / ZQ
    fp2_t t0, t1, t2;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sqr(&t0, &t0);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&t1, &t1);
    fp2_sub(&t2, &t0, &t1);
    fp2_add(&t1, &t1, &t1);
    fp2_mul(&Q->x, &t0, &t1);
    fp2_add(&Q->z, &t1, &t2);
    fp2_mul(&Q->z, &Q->z, &t2);
}

void
_xpoint_eqw_fp2_dbl_A24(
    xpoint_eqw_fp2_t *Q,
    const xpoint_eqw_fp2_t *P,
    const xpoint_eqw_fp2_t *A24,
    const bool A24_normalized
)
{
    // Doubling of a (X : Z)-point (x(P) = X / Z)
    // Input:  P = (XP : ZP) and Montgomery curve constants A24 = (A + 2C : 4C)
    // Output: Q = (XQ : ZQ) such that x(2P) = XQ / ZQ
    fp2_t t0, t1, t2;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sqr(&t0, &t0);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&t1, &t1);
    fp2_sub(&t2, &t0, &t1);
    if (!A24_normalized)
        fp2_mul(&t1, &t1, &A24->z);
    fp2_mul(&Q->x, &t0, &t1);
    fp2_mul(&t0, &t2, &A24->x);
    fp2_add(&t0, &t0, &t1);
    fp2_mul(&Q->z, &t0, &t2);
}

void
_xpoint_eqw_fp2_dbl(
    xpoint_eqw_fp2_t *Q,
    const xpoint_eqw_fp2_t *P,
    const xpoint_eqw_fp2_t *AC
)
{
    // Doubling of a (X : Z)-point (x(P) = X / Z)
    // Input:  P = (XP : ZP) and Montgomery curve constants (A : C)
    // Output: Q = (XQ : ZQ) such that x(2P) = XQ / ZQ
    fp2_t t0, t1, t2, t3;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sqr(&t0, &t0);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&t1, &t1);
    fp2_sub(&t2, &t0, &t1);
    fp2_add(&t3, &AC->z, &AC->z);
    fp2_mul(&t1, &t1, &t3);
    fp2_add(&t1, &t1, &t1);
    fp2_mul(&Q->x, &t0, &t1);
    fp2_add(&t0, &t3, &AC->x);
    fp2_mul(&t0, &t0, &t2);
    fp2_add(&t0, &t0, &t1);
    fp2_mul(&Q->z, &t0, &t2);
}

void
xpoint_eqw_fp2_add(
    xpoint_eqw_fp2_t *R,
    const xpoint_eqw_fp2_t *P,
    const xpoint_eqw_fp2_t *Q,
    const xpoint_eqw_fp2_t *PmQ
) {
    // Differential addition of (X : Z)-points (x(P) = X / Z)
    // Input:  P = (XP : ZP), Q = (XQ : ZQ), and PmQ = P - Q = (XPmQ : ZPmQ)
    // Output: R = = (XR : ZR) such that x(P + Q) = XR / ZR
    fp2_t t0, t1, t2, t3;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_add(&t2, &Q->x, &Q->z);
    fp2_sub(&t3, &Q->x, &Q->z);
    fp2_mul(&t0, &t0, &t3);
    fp2_mul(&t1, &t1, &t2);
    fp2_add(&t2, &t0, &t1);
    fp2_sub(&t3, &t0, &t1);
    fp2_sqr(&t2, &t2);
    fp2_sqr(&t3, &t3);
    fp2_mul(&t2, &PmQ->z, &t2);
    fp2_mul(&R->z, &PmQ->x, &t3);
    fp2_copy(&R->x, &t2);
}

void
_xpoint_eqw_fp2_dbladd(xpoint_eqw_fp2_t *R,
        xpoint_eqw_fp2_t *S,
        const xpoint_eqw_fp2_t *P,
        const xpoint_eqw_fp2_t *Q,
        const xpoint_eqw_fp2_t *PQ,
        const xpoint_eqw_fp2_t *A24,
        const bool A24_normalized)
{ // Simultaneous doubling and differential addition.
  // Input:  projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, the difference
  //         PQ=P-Q=(XPQ:ZPQ), and the Montgomery curve constants A24 = (A+2C:4C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery points R <- 2*P = (XR:ZR) such that x(2P)=XR/ZR, and S <- P+Q = (XS:ZS) such that =
  //         x(Q+P)=XS/ZS.
    fp2_t t0, t1, t2;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&R->x, &t0);
    fp2_sub(&t2, &Q->x, &Q->z);
    fp2_add(&S->x, &Q->x, &Q->z);
    fp2_mul(&t0, &t0, &t2);
    fp2_sqr(&R->z, &t1);
    fp2_mul(&t1, &t1, &S->x);
    fp2_sub(&t2, &R->x, &R->z);
    if (!A24_normalized)
        fp2_mul(&R->z, &R->z, &A24->z);
    fp2_mul(&R->x, &R->x, &R->z);
    fp2_mul(&S->x, &A24->x, &t2);
    fp2_sub(&S->z, &t0, &t1);
    fp2_add(&R->z, &R->z, &S->x);
    fp2_add(&S->x, &t0, &t1);
    fp2_mul(&R->z, &R->z, &t2);
    fp2_sqr(&S->z, &S->z);
    fp2_sqr(&S->x, &S->x);
    fp2_mul(&S->z, &S->z, &PQ->x);
    fp2_mul(&S->x, &S->x, &PQ->z);
}

void
_xpoint_eqw_fp2_mul(
    xpoint_eqw_fp2_t *Q,
    xpoint_eqw_fp2_t *S,
    const xpoint_eqw_fp2_t *P,
    const digit_t *k,
    const int kbits,
    const curve_mg_fp2_t *curve
)
{ // The Montgomery ladder
  // Input: projective Montgomery point P=(XP:ZP) such that xP=XP/ZP, a scalar k of bitlength kbits, and
  //        the Montgomery curve constants (A:C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery points Q <- k*P = (XQ:ZQ) such that x(k*P)=XQ/ZQ,
  //         projective Montgomery points S <- (k+1)*P = (XS:ZS) such that x((k+1)*P)=XS/ZS
  // Ref: Algorithm 4, https://eprint.iacr.org/2017/212 (The variant that also returns x1)
    xpoint_eqw_fp2_t R0, R1, A24;
    digit_t mask;
    unsigned int bit, prevbit = 0, swap;

    if (!curve->is_A24_computed_and_normalized) {
        // Computation of A24=(A+2C:4C)
        fp2_add(&A24.x, &curve->C, &curve->C);
        fp2_add(&A24.z, &A24.x, &A24.x);
        fp2_add(&A24.x, &A24.x, &curve->A);
    } else {
        fp2_copy(&A24.x, &curve->A24.x);
        fp2_copy(&A24.z, &curve->A24.z);
        // Assert A24 has been normalised
        assert(fp2_is_one(&A24.z));
    }

    // R0 <- (1:0), R1 <- P
    xpoint_eqw_fp2_init(&R0);
    fp2_copy(&R1.x, &P->x);
    fp2_copy(&R1.z, &P->z);

    // Main loop
    for (int i = kbits - 1; i >= 0; i--) {
        bit = (k[i >> LOG2RADIX] >> (i & (RADIX - 1))) & 1;
        swap = bit ^ prevbit;
        prevbit = bit;
        mask = 0 - (digit_t)swap;

        xpoint_eqw_fp2_cswap(&R0, &R1, mask);
        _xpoint_eqw_fp2_dbladd(&R0, &R1, &R0, &R1, P, &A24, true);
    }

    swap = 0 ^ prevbit;
    mask = 0 - (digit_t)swap;
    xpoint_eqw_fp2_cswap(&R0, &R1, mask);

    fp2_copy(&Q->x, &R0.x);
    fp2_copy(&Q->z, &R0.z);
    fp2_copy(&S->x, &R1.x);
    fp2_copy(&S->z, &R1.z);
}

void
_xpoint_eqw_fp2_dbl_iter(
    xpoint_eqw_fp2_t *Q,
    xpoint_eqw_fp2_t *S,
    const xpoint_eqw_fp2_t *P,
    const int k,
    const curve_mg_fp2_t *curve
)
{ // The Montgomery ladder
  // Input: projective Montgomery point P=(XP:ZP) such that xP=XP/ZP, a scalar k of bitlength kbits, and
  //        the Montgomery curve constants (A:C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery points Q <- k*P = (XQ:ZQ) such that x(k*P)=XQ/ZQ,
  //         projective Montgomery points S <- (k+1)*P = (XS:ZS) such that x((k+1)*P)=XS/ZS
  // Ref: Algorithm 4, https://eprint.iacr.org/2017/212 (The variant that also returns x1)
    xpoint_eqw_fp2_t R0, R1, A24;
    digit_t mask;
    unsigned int bit, prevbit = 0, swap;

    if (!curve->is_A24_computed_and_normalized) {
        // Computation of A24=(A+2C:4C)
        fp2_add(&A24.x, &curve->C, &curve->C);
        fp2_add(&A24.z, &A24.x, &A24.x);
        fp2_add(&A24.x, &A24.x, &curve->A);
    } else {
        fp2_copy(&A24.x, &curve->A24.x);
        fp2_copy(&A24.z, &curve->A24.z);
        // Assert A24 has been normalised
        assert(fp2_is_one(&A24.z));
    }

    // R0 <- (1:0), R1 <- P
    xpoint_eqw_fp2_init(&R0);
    fp2_copy(&R1.x, &P->x);
    fp2_copy(&R1.z, &P->z);

    // Main loop
    for (int i = 0; i < k + 1; i++) {
        bit = (i == 0) ? 1 : 0;
        swap = bit ^ prevbit;
        prevbit = bit;
        mask = 0 - (digit_t)swap;

        xpoint_eqw_fp2_cswap(&R0, &R1, mask);
        _xpoint_eqw_fp2_dbladd(&R0, &R1, &R0, &R1, P, &A24, true);
    }

    swap = 0 ^ prevbit;
    mask = 0 - (digit_t)swap;
    xpoint_eqw_fp2_cswap(&R0, &R1, mask);

    fp2_copy(&Q->x, &R0.x);
    fp2_copy(&Q->z, &R0.z);
    fp2_copy(&S->x, &R1.x);
    fp2_copy(&S->z, &R1.z);
}

int
_xpoint_eqw_fp2_dblMUL(xpoint_eqw_fp2_t *S,
        const xpoint_eqw_fp2_t *P,
        const digit_t *k,
        const xpoint_eqw_fp2_t *Q,
        const digit_t *l,
        const xpoint_eqw_fp2_t *PQ,
        const int kbits,
        const curve_mg_fp2_t *curve)
{ // The Montgomery biladder
  // Input:  projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, scalars k and l of
  //         bitlength kbits, the difference PQ=P-Q=(XPQ:ZPQ), and the Montgomery curve constants (A:C).
  // Output: projective Montgomery point S <- k*P + l*Q = (XS:ZS) such that x(k*P + l*Q)=XS/ZS.

    int i, A_is_zero;
    digit_t evens, mevens, bitk0, bitl0, maskk, maskl, temp, bs1_ip1, bs2_ip1, bs1_i, bs2_i, h;
    digit_t sigma[2] = { 0 }, pre_sigma = 0;
    digit_t k_t[NWORDS_ORDER], l_t[NWORDS_ORDER], one[NWORDS_ORDER] = { 0 }, r[2 * BITS] = { 0 };
    xpoint_eqw_fp2_t DIFF1a, DIFF1b, DIFF2a, DIFF2b, R[3] = { 0 }, T[3];

    // differential additions formulas are invalid in this case
    if (xpoint_eqw_fp2_has_zero_coordinate(P) | xpoint_eqw_fp2_has_zero_coordinate(Q) | xpoint_eqw_fp2_has_zero_coordinate(PQ))
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
    xpoint_eqw_fp2_init(&R[0]);
    maskk = 0 - sigma[0];
    curve_mg_fp2_select_point(&R[1], P, Q, maskk);
    curve_mg_fp2_select_point(&R[2], Q, P, maskk);

    fp2_copy(&DIFF1a.x, &R[1].x);
    fp2_copy(&DIFF1a.z, &R[1].z);
    fp2_copy(&DIFF1b.x, &R[2].x);
    fp2_copy(&DIFF1b.z, &R[2].z);

    // Initialize DIFF2a <- P+Q, DIFF2b <- P-Q
    xpoint_eqw_fp2_add(&R[2], &R[1], &R[2], PQ);
    if (xpoint_eqw_fp2_has_zero_coordinate(&R[2]))
        return 0; // non valid formulas

    fp2_copy(&DIFF2a.x, &R[2].x);
    fp2_copy(&DIFF2a.z, &R[2].z);
    fp2_copy(&DIFF2b.x, &PQ->x);
    fp2_copy(&DIFF2b.z, &PQ->z);

    A_is_zero = fp2_is_zero(&curve->A);

    // Main loop
    for (i = kbits - 1; i >= 0; i--) {
        h = r[2 * i] + r[2 * i + 1]; // in {0, 1, 2}
        maskk = 0 - (h & 1);
        curve_mg_fp2_select_point(&T[0], &R[0], &R[1], maskk);
        maskk = 0 - (h >> 1);
        curve_mg_fp2_select_point(&T[0], &T[0], &R[2], maskk);
        if (A_is_zero) {
            _xpoint_eqw_fp2_dbl_E0(&T[0], &T[0]);
        } else {
            assert(fp2_is_one(&curve->A24.z));
            _xpoint_eqw_fp2_dbl_A24(&T[0], &T[0], &curve->A24, true);
        }

        maskk = 0 - r[2 * i + 1]; // in {0, 1}
        curve_mg_fp2_select_point(&T[1], &R[0], &R[1], maskk);
        curve_mg_fp2_select_point(&T[2], &R[1], &R[2], maskk);

        xpoint_eqw_fp2_cswap(&DIFF1a, &DIFF1b, maskk);
        xpoint_eqw_fp2_add(&T[1], &T[1], &T[2], &DIFF1a);
        xpoint_eqw_fp2_add(&T[2], &R[0], &R[2], &DIFF2a);

        // If hw (mod 2) = 1 then swap DIFF2a and DIFF2b
        maskk = 0 - (h & 1);
        xpoint_eqw_fp2_cswap(&DIFF2a, &DIFF2b, maskk);

        // R <- T
        xpoint_eqw_fp2_copy(&R[0], &T[0]);
        xpoint_eqw_fp2_copy(&R[1], &T[1]);
        xpoint_eqw_fp2_copy(&R[2], &T[2]);
    }

    // Output R[evens]
    curve_mg_fp2_select_point(S, &R[0], &R[1], mevens);

    maskk = 0 - (bitk0 & bitl0);
    curve_mg_fp2_select_point(S, S, &R[2], maskk);
    return 1;
}

// WRAPPERS to export

void
xpoint_eqw_fp2_dbl(xpoint_eqw_fp2_t *res, const xpoint_eqw_fp2_t *P, const curve_mg_fp2_t *curve)
{
    // If A24 = ((A+2)/4 : 1) we save multiplications
    if (curve->is_A24_computed_and_normalized) {
        assert(fp2_is_one(&curve->A24.z));
        _xpoint_eqw_fp2_dbl_A24(res, P, &curve->A24, true);
    } else {
        // Otherwise we compute A24 on the fly for doubling
        _xpoint_eqw_fp2_dbl(res, P, (const xpoint_eqw_fp2_t *)curve);
    }
}

void
xpoint_eqw_fp2_dbl_iter(xpoint_eqw_fp2_t *res, int n, const xpoint_eqw_fp2_t *P, curve_mg_fp2_t *curve)
{
    if (n == 0) {
        xpoint_eqw_fp2_copy(res, P);
        return;
    }

    // When the chain is long enough, we should normalise A24
    if (n > 50) {
        curve_mg_fp2_normalise_A24(curve);
    }

    // When A24 is normalized we can save some multiplications
    if (curve->is_A24_computed_and_normalized) {
        assert(fp2_is_one(&curve->A24.z));
        _xpoint_eqw_fp2_dbl_A24(res, P, &curve->A24, true);
        for (int i = 0; i < n - 1; i++) {
            assert(fp2_is_one(&curve->A24.z));
            _xpoint_eqw_fp2_dbl_A24(res, res, &curve->A24, true);
        }
    } else {
        // Otherwise we do normal doubling
        _xpoint_eqw_fp2_dbl(res, P, (const xpoint_eqw_fp2_t *)curve);
        for (int i = 0; i < n - 1; i++) {
            _xpoint_eqw_fp2_dbl(res, res, (const xpoint_eqw_fp2_t *)curve);
        }
    }
}

void
xpoint_eqw_fp2_mul(xpoint_eqw_fp2_t *res, xpoint_eqw_fp2_t *resp1, const digit_t *scalar, const int kbits, const xpoint_eqw_fp2_t *P, curve_mg_fp2_t *curve)
{
    // For large scalars it's worth normalising anyway
    if (kbits > 50) {
        curve_mg_fp2_normalise_A24(curve);
    }

    // When A24 is computed and normalized we save some Fp2 multiplications
    _xpoint_eqw_fp2_mul(res, resp1, P, scalar, kbits, curve);
}

void
xpoint_eqw_fp2_mul_dbl_iter(
    xpoint_eqw_fp2_t *res,
    xpoint_eqw_fp2_t *resp1,
    const int k,
    const xpoint_eqw_fp2_t *P,
    curve_mg_fp2_t *curve
) {
    // For large scalars it's worth normalising anyway
    if (k > 50)
        curve_mg_fp2_normalise_A24(curve);

    // When A24 is computed and normalized we save some Fp2 multiplications
    _xpoint_eqw_fp2_dbl_iter(res, resp1, P, k, curve);
}

void
sw_curve_normalise_fp2(curve_sw_fp2_t *E2, const curve_sw_fp2_t *E1) {
    fp2_t C_inv, C_inv_pow2;
    fp2_copy(&C_inv, &E1->C);
    fp2_inv(&C_inv);
    fp2_sqr(&C_inv_pow2, &C_inv);
    fp2_mul(&E2->A, &E1->A, &C_inv_pow2);
    fp2_mul(&E2->B, &E1->B, &C_inv_pow2);
    fp2_mul(&E2->B, &E2->B, &C_inv);
    fp2_set_one(&E2->C);
}

void
mg_to_sw_fp2(
        curve_sw_fp2_t *E_sw,
        xpoint_eqw_fp2_t *xpts_sw,
        point_eqw_fp2_t *pts_sw,
        const curve_mg_fp2_t *E_m,
        const xpoint_eqw_fp2_t *xpts_m,
        const uint32_t len_xpts,
        const point_eqw_fp2_t *pts_m,
        const uint32_t len_pts
) {
    // Computes isomorphism from Montgomery form to short Weierstrass from
    // i.e. computes isomorphism
    //   phi: E_m: By^2 = x(x^2 + Ax + 1) -> E_w: y^2 = x^3 + ax + b
    // and maps both x-points and points through
    // Note: These points may be y-twisted, i.e. such that (x, eye * y) lies on E
    // Ref: https://eprint.iacr.org/2017/212 Sec 2.4 (Case B = 1)
    // Cost: Curve: 10a/s + 2M, Per x-point: 1a/s + 2M, Per point: 1a/s + 3M

    fp2_t A, C;
    fp2_copy(&A, &E_m->A);
    fp2_copy(&C, &E_m->C);

    fp2_t A_pow2, C_pow2, C_pow2_mul9;
    fp2_sqr(&A_pow2, &A);

    fp2_sqr(&C_pow2, &C);
    // fp2_mul_small(&C_pow2_mul9, &C_pow2, 9);
        fp2_add(&C_pow2_mul9, &C_pow2, &C_pow2);
        fp2_add(&C_pow2_mul9, &C_pow2_mul9, &C_pow2_mul9);
        fp2_add(&C_pow2_mul9, &C_pow2_mul9, &C_pow2_mul9);
        fp2_add(&C_pow2_mul9, &C_pow2_mul9, &C_pow2);

    // A_new = 9 * C^2 - 3 * A^2
    fp2_sub(&E_sw->A, &C_pow2_mul9, &A_pow2);
    fp2_sub(&E_sw->A, &E_sw->A, &A_pow2);
    fp2_sub(&E_sw->A, &E_sw->A, &A_pow2);

    // B_new = A * (2 * A^2 - 9 * C^2)
    fp2_add(&E_sw->B, &A_pow2, &A_pow2);
    fp2_sub(&E_sw->B, &E_sw->B, &C_pow2_mul9);
    fp2_mul(&E_sw->B, &E_sw->B, &A);

    // C_new = 3 * C
    // fp2_mul_small(&E_sw->C, &C, 3);
        fp2_add(&E_sw->C, &C, &C);
        fp2_add(&E_sw->C, &E_sw->C, &C);

    fp2_t *const C_mul3 = &E_sw->C;

    fp2_t tmp;

    for (uint32_t i = 0; i < len_xpts; i++) {
        // (Px_new, Pz_new) = (3 * C * Px + A * Pz, 3 * C * Pz)
        fp2_mul(&(xpts_sw + i)->x, C_mul3, &(xpts_m + i)->x);
        fp2_mul(&tmp, &A, &(xpts_m + i)->z);
        fp2_add(&(xpts_sw + i)->x, &(xpts_sw + i)->x, &tmp);
        fp2_mul(&(xpts_sw + i)->z, C_mul3, &(xpts_m + i)->z);
    }

    for (uint32_t i = 0; i < len_pts; i++) {
        // (Px_new, Py_new, Pz_new) = (3 * C * Px + A * Pz, 3 * C * Py, 3 * C * Pz)
        fp2_mul(&(pts_sw + i)->x, C_mul3, &(pts_m + i)->x);
        fp2_mul(&tmp, &A, &(pts_m + i)->z);
        fp2_add(&(pts_sw + i)->x, &(pts_sw + i)->x, &tmp);
        fp2_mul(&(pts_sw + i)->y, C_mul3, &(pts_m + i)->y);
        fp2_mul(&(pts_sw + i)->z, C_mul3, &(pts_m + i)->z);
    }
}

void
sw_to_mg_fp2(
        curve_mg_fp2_t *E_m,
        xpoint_eqw_fp2_t *xpts_m,
        point_eqw_fp2_t *pts_m,
        const curve_sw_fp2_t *E_sw,
        const xpoint_eqw_fp2_t *xT8,
        const xpoint_eqw_fp2_t *xpts_sw,
        const uint32_t len_xpts,
        const point_eqw_fp2_t *pts_sw,
        const uint32_t len_pts
) {
    xpoint_eqw_fp2_t alpha_pt;

    fp2_t C_pow2, C_pow3;
    fp2_sqr(&C_pow2, &E_sw->C);
    fp2_mul(&C_pow3, &C_pow2, &E_sw->C);

    sw_xdouble_fp2(&alpha_pt, E_sw, &C_pow2, &C_pow3, xT8);
    sw_xdouble_fp2(&alpha_pt, E_sw, &C_pow2, &C_pow3, &alpha_pt);

    fp2_t a, alpha, beta_pow2, beta, beta_sqrt;

    // Normalise alpha and E.a
    fp2_div(&alpha, &alpha_pt.x, &alpha_pt.z);
    // a = E.a / E.c^2
    // (We won't need normalised E.b, so we only do E.a manually)
    fp2_sqr(&a, &E_sw->C);
    fp2_div(&a, &E_sw->A, &a);

    #ifndef NDEBUG
        // Verify that alpha^3 + (E.A / E.C^2) * alpha + (E.B / E.C3) == 0
        // i.e. alpha is x-coordinate of 2-torsion point
        fp2_t tmp1, tmp2, tmp3;
        fp2_sqr(&tmp1, &alpha);
        fp2_mul(&tmp1, &tmp1, &alpha);

        fp2_div(&tmp2, &E_sw->A, &C_pow2);
        fp2_mul(&tmp2, &alpha, &tmp2);

        fp2_div(&tmp3, &E_sw->B, &C_pow3);
        fp2_add(&tmp1, &tmp1, &tmp2);
        fp2_add(&tmp1, &tmp1, &tmp3);
        assert(fp2_is_zero(&tmp1));
    #endif

    // beta_pow2 = 3 * alpha^2 + a
    fp2_sqr(&beta_pow2, &alpha);
    // fp2_mul_small(&beta_pow2, &beta_pow2, 3);
        fp2_add(&beta, &beta_pow2, &beta_pow2);
        fp2_add(&beta_pow2, &beta_pow2, &beta);
    fp2_add(&beta_pow2, &beta_pow2, &a);

    fp2_copy(&beta, &beta_pow2);

    assert(fp_is_zero(&beta.im));
    fp2_copy(&beta, &beta_pow2);
    fp2_sqrt(&beta);
    assert(fp_is_zero(&beta.im));

    if (!fp_is_square(&beta.re))
        fp_neg(&beta.re, &beta.re);
    assert(fp_is_square(&beta.re));

    assert(fp_is_zero(&beta.im));
    fp2_copy(&beta_sqrt, &beta);
    fp2_sqrt(&beta_sqrt);
    assert(fp_is_zero(&beta_sqrt.im));

    // fp2_mul_small(&E_m->A, &alpha, 3);
        fp2_add(&E_m->C, &alpha, &alpha);
        fp2_add(&E_m->A, &E_m->C, &alpha);
    fp2_copy(&E_m->C, &beta);

    for (uint32_t i = 0; i < len_xpts; i++) {
        // x, z = x - alpha * z, z * beta
        fp2_mul(&(xpts_m + i)->x, &alpha, &(xpts_sw + i)->z);
        fp2_sub(&(xpts_m + i)->x, &(xpts_sw + i)->x, &(xpts_m + i)->x);
        fp2_mul(&(xpts_m + i)->z, &beta, &(xpts_sw + i)->z);
    }

    for (uint32_t i = 0; i < len_pts; i++) {
        // x, y, z = (x - alpha * z) * beta, y * beta_sqrt, z * beta_pow2
        fp2_mul(&(pts_m + i)->x, &alpha, &(pts_sw + i)->z);
        fp2_sub(&(pts_m + i)->x, &(pts_sw + i)->x, &(pts_m + i)->x);
        fp2_mul(&(pts_m + i)->x, &(pts_m + i)->x, &beta);
        fp2_mul(&(pts_m + i)->y, &beta_sqrt, &(pts_sw + i)->y);
        fp2_mul(&(pts_m + i)->z, &beta_pow2, &(pts_sw + i)->z);
    }
}


void
point_eqw_fp2_neg(point_eqw_fp2_t *Q, point_eqw_fp2_t *P) {
    fp2_copy(&Q->x, &P->x);
    fp2_neg(&Q->y, &P->y);
    fp2_copy(&Q->z, &P->z);
}
