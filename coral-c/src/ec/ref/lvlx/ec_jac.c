#include <assert.h>
#include "ec.h"

void
point_jac_fp2_init(point_jac_fp2_t *P)
{ // Initialize Montgomery in Jacobian coordinates as identity element (0:1:0)
    fp2_set_zero(&P->x);
    fp2_set_one(&P->y);
    fp2_set_zero(&P->z);
}

void
point_jac_fp2_copy(point_jac_fp2_t *P, const point_jac_fp2_t *Q)
{
    fp2_copy(&(P->x), &(Q->x));
    fp2_copy(&(P->y), &(Q->y));
    fp2_copy(&(P->z), &(Q->z));
}

void
point_jac_fp2_neg(point_jac_fp2_t *Q, const point_jac_fp2_t *P)
{
    fp2_copy(&Q->x, &P->x);
    fp2_neg(&Q->y, &P->y);
    fp2_copy(&Q->z, &P->z);
}

uint32_t
point_jac_fp2_is_equal(const point_jac_fp2_t *P, const point_jac_fp2_t *Q)
{ // Evaluate if two points in Jacobian coordinates (X:Y:Z) are equal
  // Returns 1 (true) if P=Q, 0 (false) otherwise
    fp2_t t0, t1, t2, t3;

    fp2_sqr(&t0, &Q->z);
    fp2_mul(&t2, &P->x, &t0); // x1*z2^2
    fp2_sqr(&t1, &P->z);
    fp2_mul(&t3, &Q->x, &t1); // x2*z1^2
    fp2_sub(&t2, &t2, &t3);

    fp2_mul(&t0, &t0, &Q->z);
    fp2_mul(&t0, &P->y, &t0); // y1*z2^3
    fp2_mul(&t1, &t1, &P->z);
    fp2_mul(&t1, &Q->y, &t1); // y2*z1^3
    fp2_sub(&t0, &t0, &t1);

    return fp2_is_zero(&t0) & fp2_is_zero(&t2);
}

void
point_jac_fp2_to_ec(xpoint_eqw_fp2_t *P, const point_jac_fp2_t *xyP)
{
    fp2_copy(&P->x, &xyP->x);
    fp2_copy(&P->z, &xyP->z);
    fp2_sqr(&P->z, &P->z);

    // If xyP = (0:1:0), we currently have P=(0 : 0) but we want to set P=(1:0)
    uint32_t c1, c2;
    fp2_t one;
    fp2_set_one(&one);

    c1 = fp2_is_zero(&P->x);
    c2 = fp2_is_zero(&P->z);
    fp2_select(&P->x, &P->x, &one, c1 & c2);
}

// 2017/212 Sec 2.4
// @param [OUTPUT] Q: The point P in on the short weierstass curve isomorphic to E
// @param [OUTPUT] T: The value T = a * Z^4
// @param [OUTPUT] A_div3: The value E.A / 3
// @param [INPUT] P: A point P in jacobian coordinates
// @param [INPUT] E: The montgomery curve on which P lies (assumed normalised)
void
jac_to_ws_fp2(point_jac_fp2_t *Q, fp2_t *T, fp2_t *A_div3, const point_jac_fp2_t *P, const curve_mg_fp2_t *E)
{
    // Cost of 3M + 2S when A != 0.

    // a = 1 - A^2/3
    // Qx = Px + A * Pz^2 / 3
    // Qy = Py
    // Qz = Pz
    // T = a * Pz^4

    fp_t one;
    fp_set_one(&one);
    fp2_t a;

    point_jac_fp2_copy(Q, P);

    if (!fp2_is_zero(&(E->A))) {
        // Compute A_div3
        fp_div3(&(A_div3->re), &(E->A.re));
        fp_div3(&(A_div3->im), &(E->A.im));

        // Qx = Px + A * Pz^2 /3
        fp2_sqr(T, &P->z);
        fp2_mul(&Q->x, A_div3, T);
        fp2_add(&Q->x, &Q->x, &P->x);

        // a = 1 - A^2 / 3
        fp2_mul(&a, A_div3, &E->A);
        fp_sub(&a.re, &one, &a.re);
        fp_neg(&a.im, &(a.im));

        // T = a * Pz^4
        fp2_sqr(T, T);
        fp2_mul(T, T, &a);

    } else {
        // a == 1
        // T = a * Pz^4
        fp2_sqr(T, &P->z);
        fp2_sqr(T, T);
    }
}

// 2017/212 Sec 2.4
// @param [OUTPUT] Q: The point P in jacobian coordinates in montgomery form
// @param [INPUT] P: A point P in weierstrass (equally weighted) coordinates
// @param [INPUT] A_div3: The value E.A / 3
// @param [INPUT] E: The weierstrass curve on which P lies (assumed normalised)
void
jac_from_ws_fp2(point_jac_fp2_t *Q, const point_jac_fp2_t *P, const fp2_t *A_div3, const curve_mg_fp2_t *E)
{
    // Cost of 1M + 1S when A != 0.
    // Qx = Px - A * Pz^2 / 3
    // Qy = Py
    // Qz = Pz

    fp2_t T;
    point_jac_fp2_copy(Q, P);

    if (!fp2_is_zero(&(E->A))) {
        fp2_sqr(&T, &P->z);
        fp2_mul(&T, &T, A_div3);
        fp2_sub(&Q->x, &P->x, &T);
    }
}

void
point_jac_fp2_dbl(point_jac_fp2_t *Q, const point_jac_fp2_t *P, const curve_mg_fp2_t *AC)
{ // Cost of 6M + 6S.
  // Doubling on a Montgomery curve, representation in Jacobian coordinates (X:Y:Z) corresponding to
  // (X/Z^2,Y/Z^3) This version receives the coefficient value A
    fp2_t t0, t1, t2, t3;

    uint32_t flag = fp2_is_zero(&P->x) & fp2_is_zero(&P->z);

    fp2_sqr(&t0, &P->x); // t0 = x1^2
    fp2_add(&t1, &t0, &t0);
    fp2_add(&t0, &t0, &t1); // t0 = 3x1^2
    fp2_sqr(&t1, &P->z);    // t1 = z1^2
    fp2_mul(&t2, &P->x, &AC->A);
    fp2_add(&t2, &t2, &t2); // t2 = 2Ax1
    fp2_add(&t2, &t1, &t2); // t2 = 2Ax1+z1^2
    fp2_mul(&t2, &t1, &t2); // t2 = z1^2(2Ax1+z1^2)
    fp2_add(&t2, &t0, &t2); // t2 = alpha = 3x1^2 + z1^2(2Ax1+z1^2)
    fp2_mul(&Q->z, &P->y, &P->z);
    fp2_add(&Q->z, &Q->z, &Q->z); // z2 = 2y1z1
    fp2_sqr(&t0, &Q->z);
    fp2_mul(&t0, &t0, &AC->A); // t0 = 4Ay1^2z1^2
    fp2_sqr(&t1, &P->y);
    fp2_add(&t1, &t1, &t1);     // t1 = 2y1^2
    fp2_add(&t3, &P->x, &P->x); // t3 = 2x1
    fp2_mul(&t3, &t1, &t3);     // t3 = 4x1y1^2
    fp2_sqr(&Q->x, &t2);        // x2 = alpha^2
    fp2_sub(&Q->x, &Q->x, &t0); // x2 = alpha^2 - 4Ay1^2z1^2
    fp2_sub(&Q->x, &Q->x, &t3);
    fp2_sub(&Q->x, &Q->x, &t3); // x2 = alpha^2 - 4Ay1^2z1^2 - 8x1y1^2
    fp2_sub(&Q->y, &t3, &Q->x); // y2 = 4x1y1^2 - x2
    fp2_mul(&Q->y, &Q->y, &t2); // y2 = alpha(4x1y1^2 - x2)
    fp2_sqr(&t1, &t1);          // t1 = 4y1^4
    fp2_sub(&Q->y, &Q->y, &t1);
    fp2_sub(&Q->y, &Q->y, &t1); // y2 = alpha(4x1y1^2 - x2) - 8y1^4

    fp2_select(&Q->x, &Q->x, &P->x, -flag);
    fp2_select(&Q->z, &Q->z, &P->z, -flag);
}

void
point_jac_fp2_dblw(point_jac_fp2_t *Q, fp2_t *u, const point_jac_fp2_t *P, const fp2_t *t)
{ // Cost of 3M + 5S.
  // Doubling on a Weierstrass curve, representation in modified Jacobian coordinates
  // (X:Y:Z:T=a*Z^4) corresponding to (X/Z^2,Y/Z^3), where a is the curve coefficient.
  // Formula from https://hyperelliptic.org/EFD/g1p/auto-shortw-modified.html

    uint32_t flag = fp2_is_zero(&P->x) & fp2_is_zero(&P->z);

    fp2_t xx, c, cc, r, s, m;
    // XX = X^2
    fp2_sqr(&xx, &P->x);
    // A = 2*Y^2
    fp2_sqr(&c, &P->y);
    fp2_add(&c, &c, &c);
    // AA = A^2
    fp2_sqr(&cc, &c);
    // R = 2*AA
    fp2_add(&r, &cc, &cc);
    // S = (X+A)^2-XX-AA
    fp2_add(&s, &P->x, &c);
    fp2_sqr(&s, &s);
    fp2_sub(&s, &s, &xx);
    fp2_sub(&s, &s, &cc);
    // M = 3*XX+T1
    fp2_add(&m, &xx, &xx);
    fp2_add(&m, &m, &xx);
    fp2_add(&m, &m, t);
    // X3 = M^2-2*S
    fp2_sqr(&Q->x, &m);
    fp2_sub(&Q->x, &Q->x, &s);
    fp2_sub(&Q->x, &Q->x, &s);
    // Z3 = 2*Y*Z
    fp2_mul(&Q->z, &P->y, &P->z);
    fp2_add(&Q->z, &Q->z, &Q->z);
    // Y3 = M*(S-X3)-R
    fp2_sub(&Q->y, &s, &Q->x);
    fp2_mul(&Q->y, &Q->y, &m);
    fp2_sub(&Q->y, &Q->y, &r);
    // T3 = 2*R*T1
    fp2_mul(u, t, &r);
    fp2_add(u, u, u);

    fp2_select(&Q->x, &Q->x, &P->x, -flag);
    fp2_select(&Q->z, &Q->z, &P->z, -flag);
}

void
point_jac_fp2_select(point_jac_fp2_t *Q, const point_jac_fp2_t *P1, const point_jac_fp2_t *P2, const digit_t option)
{ // Select points
  // If option = 0 then Q <- P1, else if option = 0xFF...FF then Q <- P2
    fp2_select(&(Q->x), &(P1->x), &(P2->x), option);
    fp2_select(&(Q->y), &(P1->y), &(P2->y), option);
    fp2_select(&(Q->z), &(P1->z), &(P2->z), option);
}

void
point_jac_fp2_add(point_jac_fp2_t *R, const point_jac_fp2_t *P, const point_jac_fp2_t *Q, const curve_mg_fp2_t *AC)
{
    // Addition on a Montgomery curve, representation in Jacobian coordinates (X:Y:Z) corresponding
    // to (x,y) = (X/Z^2,Y/Z^3) This version receives the coefficient value A
    //
    // Complete routine, to handle all edge cases:
    //   if ZP == 0:            # P == inf
    //       return Q
    //   if ZQ == 0:            # Q == inf
    //       return P
    //   dy <- YQ*ZP**3 - YP*ZQ**3
    //   dx <- XQ*ZP**2 - XP*ZQ**2
    //   if dx == 0:             # x1 == x2
    //       if dy == 0:         # ... and y1 == y2: doubling case
    //           dy <- ZP*ZQ * (3*XP^2 + ZP^2 * (2*A*XP + ZP^2))
    //           dx <- 2*YP*ZP
    //       else:              # ... but y1 != y2, thus P = -Q
    //           return inf
    //   XR <- dy**2 - dx**2 * (A*ZP^2*ZQ^2 + XP*ZQ^2 + XQ*ZP^2)
    //   YR <- dy * (XP*ZQ^2 * dx^2 - XR) - YP*ZQ^3 * dx^3
    //   ZR <- dx * ZP * ZQ

    // Constant time processing:
    // - The case for P == 0 or Q == 0 is handled at the end with conditional select
    // - dy and dx are computed for both the normal and doubling cases, we switch when
    //   dx == dy == 0 for the normal case.
    // - If we have that P = -Q then dx = 0 and so ZR will be zero, giving us the point
    //   at infinity for "free".
    //
    // These current formula are expensive and I'm probably missing some tricks...
    // Thought I'd get the ball rolling.
    // Cost 17M + 6S + 13a
    fp2_t t0, t1, t2, t3, u1, u2, v1, dx, dy;

    /* If P is zero or Q is zero we will conditionally swap before returning. */
    uint32_t ctl1 = fp2_is_zero(&P->z);
    uint32_t ctl2 = fp2_is_zero(&Q->z);

    /* Precompute some values */
    fp2_sqr(&t0, &P->z); // t0 = z1^2
    fp2_sqr(&t1, &Q->z); // t1 = z2^2

    /* Compute dy and dx for ordinary case */
    fp2_mul(&v1, &t1, &Q->z); // v1 = z2^3
    fp2_mul(&t2, &t0, &P->z); // t2 = z1^3
    fp2_mul(&v1, &v1, &P->y); // v1 = y1z2^3
    fp2_mul(&t2, &t2, &Q->y); // t2 = y2z1^3
    fp2_sub(&dy, &t2, &v1);   // dy = y2z1^3 - y1z2^3
    fp2_mul(&u2, &t0, &Q->x); // u2 = x2z1^2
    fp2_mul(&u1, &t1, &P->x); // u1 = x1z2^2
    fp2_sub(&dx, &u2, &u1);   // dx = x2z1^2 - x1z2^2

    /* Compute dy and dx for doubling case */
    fp2_add(&t1, &P->y, &P->y);   // dx_dbl = t1 = 2y1
    fp2_add(&t2, &AC->A, &AC->A); // t2 = 2A
    fp2_mul(&t2, &t2, &P->x);     // t2 = 2Ax1
    fp2_add(&t2, &t2, &t0);       // t2 = 2Ax1 + z1^2
    fp2_mul(&t2, &t2, &t0);       // t2 = z1^2 * (2Ax1 + z1^2)
    fp2_sqr(&t0, &P->x);          // t0 = x1^2
    fp2_add(&t2, &t2, &t0);       // t2 = x1^2 + z1^2 * (2Ax1 + z1^2)
    fp2_add(&t2, &t2, &t0);       // t2 = 2*x1^2 + z1^2 * (2Ax1 + z1^2)
    fp2_add(&t2, &t2, &t0);       // t2 = 3*x1^2 + z1^2 * (2Ax1 + z1^2)
    fp2_mul(&t2, &t2, &Q->z);     // dy_dbl = t2 = z2 * (3*x1^2 + z1^2 * (2Ax1 + z1^2))

    /* If dx is zero and dy is zero swap with double variables */
    uint32_t ctl = fp2_is_zero(&dx) & fp2_is_zero(&dy);
    fp2_select(&dx, &dx, &t1, ctl);
    fp2_select(&dy, &dy, &t2, ctl);

    /* Some more precomputations */
    fp2_mul(&t0, &P->z, &Q->z); // t0 = z1z2
    fp2_sqr(&t1, &t0);          // t1 = z1z2^2
    fp2_sqr(&t2, &dx);          // t2 = dx^2
    fp2_sqr(&t3, &dy);          // t3 = dy^2

    /* Compute x3 = dy**2 - dx**2 * (A*ZP^2*ZQ^2 + XP*ZQ^2 + XQ*ZP^2) */
    fp2_mul(&R->x, &AC->A, &t1); // x3 = A*(z1z2)^2
    fp2_add(&R->x, &R->x, &u1);  // x3 = A*(z1z2)^2 + u1
    fp2_add(&R->x, &R->x, &u2);  // x3 = A*(z1z2)^2 + u1 + u2
    fp2_mul(&R->x, &R->x, &t2);  // x3 = dx^2 * (A*(z1z2)^2 + u1 + u2)
    fp2_sub(&R->x, &t3, &R->x);  // x3 = dy^2 - dx^2 * (A*(z1z2)^2 + u1 + u2)

    /* Compute y3 = dy * (XP*ZQ^2 * dx^2 - XR) - YP*ZQ^3 * dx^3*/
    fp2_mul(&R->y, &u1, &t2);     // y3 = u1 * dx^2
    fp2_sub(&R->y, &R->y, &R->x); // y3 = u1 * dx^2 - x3
    fp2_mul(&R->y, &R->y, &dy);   // y3 = dy * (u1 * dx^2 - x3)
    fp2_mul(&t3, &t2, &dx);       // t3 = dx^3
    fp2_mul(&t3, &t3, &v1);       // t3 = v1 * dx^3
    fp2_sub(&R->y, &R->y, &t3);   // y3 = dy * (u1 * dx^2 - x3) - v1 * dx^3

    /* Compute z3 = dx * z1 * z2 */
    fp2_mul(&R->z, &dx, &t0);

    /* Finally, we need to set R = P is Q.Z = 0 and R = Q if P.Z = 0 */
    point_jac_fp2_select(R, R, Q, ctl1);
    point_jac_fp2_select(R, R, P, ctl2);
}

void
point_jac_fp2_sub(
    point_jac_fp2_t *PmQ,
    const point_jac_fp2_t *P,
    const point_jac_fp2_t *Q,
    const curve_mg_fp2_t *E
) {
    point_jac_fp2_t tmp;
    point_jac_fp2_neg(&tmp, Q);
    point_jac_fp2_add(PmQ, P, &tmp, E);
}

void
point_jac_fp2_to_xz_add_components(
    add_components_fp2_t *add_comp,
    const point_jac_fp2_t *P,
    const point_jac_fp2_t *Q,
    const curve_mg_fp2_t *AC
) {
    // Take P and Q in E distinct, two point_jac_fp2_t, return three components u,v and w in Fp2 such
    // that the xz coordinates of P+Q are (u-v:w) and of P-Q are (u+v:w)

    fp2_t t0, t1, t2, t3, t4, t5, t6;

    fp2_sqr(&t0, &P->z);             // t0 = z1^2
    fp2_sqr(&t1, &Q->z);             // t1 = z2^2
    fp2_mul(&t2, &P->x, &t1);        // t2 = x1z2^2
    fp2_mul(&t3, &t0, &Q->x);        // t3 = z1^2x2
    fp2_mul(&t4, &P->y, &Q->z);      // t4 = y1z2
    fp2_mul(&t4, &t4, &t1);          // t4 = y1z2^3
    fp2_mul(&t5, &P->z, &Q->y);      // t5 = z1y2
    fp2_mul(&t5, &t5, &t0);          // t5 = z1^3y2
    fp2_mul(&t0, &t0, &t1);          // t0 = (z1z2)^2
    fp2_mul(&t6, &t4, &t5);          // t6 = (z1z_2)^3y1y2
    fp2_add(&add_comp->v, &t6, &t6); // v  = 2(z1z_2)^3y1y2
    fp2_sqr(&t4, &t4);               // t4 = y1^2z2^6
    fp2_sqr(&t5, &t5);               // t5 = z1^6y_2^2
    fp2_add(&t4, &t4, &t5);          // t4 = z1^6y_2^2 + y1^2z2^6
    fp2_add(&t5, &t2, &t3);          // t5 = x1z2^2 +z_1^2x2
    fp2_add(&t6, &t3, &t3);          // t6 = 2z_1^2x2
    fp2_sub(&t6, &t5, &t6);          // t6 = lambda = x1z2^2 - z_1^2x2
    fp2_sqr(&t6, &t6);               // t6 = lambda^2 = (x1z2^2 - z_1^2x2)^2
    fp2_mul(&t1, &AC->A, &t0);       // t1 = A*(z1z2)^2
    fp2_add(&t1, &t5, &t1);          // t1 = gamma =A*(z1z2)^2 + x1z2^2 +z_1^2x2
    fp2_mul(&t1, &t1, &t6);          // t1 = gamma*lambda^2
    fp2_sub(&add_comp->u, &t4, &t1); // u  = z1^6y_2^2 + y1^2z2^6 - gamma*lambda^2
    fp2_mul(&add_comp->w, &t6, &t0); // w  = (z1z2)^2(lambda)^2
}

void
point_jac_to_eqw_fp2(point_eqw_fp2_t *P_eqw, const point_jac_fp2_t *P_jac)
{
    // (X: Y: Z) represents the affine point (X / Z^2, Y / Z^3)
    // Hence, is represented by (X * Z: Y: Z^3) in equally weighted coordinates
    if (!fp2_is_one(&P_jac->z)) {
        fp2_mul(&P_eqw->x, &P_jac->x, &P_jac->z);
        fp2_copy(&P_eqw->y, &P_jac->y);
        fp2_sqr(&P_eqw->z, &P_jac->z);
        fp2_mul(&P_eqw->z, &P_eqw->z, &P_jac->z);
    } else {
        fp2_copy(&P_eqw->x, &P_jac->x);
        fp2_copy(&P_eqw->y, &P_jac->y);
        fp2_set_one(&P_eqw->z);
    }
}

void
point_eqw_to_jac_fp2(point_jac_fp2_t *P_jac, const point_eqw_fp2_t *P_eqw)
{
    // (X: Y: Z) represents the affine point (X / Z, Y / Z)
    // Hence, is represented by (X * Z: Y * Z^2: Z) in Jacobian coordinates
    if (!fp2_is_one(&P_eqw->z)) {
        fp2_mul(&P_jac->x, &P_eqw->x, &P_eqw->z);
        fp2_sqr(&P_jac->y, &P_eqw->z);
        fp2_mul(&P_jac->y, &P_jac->y, &P_eqw->y);
        fp2_copy(&P_jac->z, &P_eqw->z);
    } else {
        fp2_copy(&P_jac->x, &P_eqw->x);
        fp2_copy(&P_jac->y, &P_eqw->y);
        fp2_set_one(&P_jac->z);
    }
}

inline void
point_eqw_to_jac_fp_to_fp2(point_jac_fp2_t *P_jac, const point_eqw_fp_t *P_eqw)
{
    // (X: Y: Z) represents the affine point (X / Z, Y / Z)
    // Hence, is represented by (X * Z: Y * Z^2: Z) in Jacobian coordinates
    if (!fp_is_one(&P_eqw->z)) {
        fp_mul(&P_jac->x.re, &P_eqw->x, &P_eqw->z);
        fp_sqr(&P_jac->y.re, &P_eqw->z);
        fp_mul(&P_jac->y.re, &P_jac->y.re, &P_eqw->y);
        fp_copy(&P_jac->z.re, &P_eqw->z);
    } else {
        fp_copy(&P_jac->x.re, &P_eqw->x);
        fp_copy(&P_jac->y.re, &P_eqw->y);
        fp_set_one(&P_jac->z.re);
    }
    fp_set_zero(&P_jac->x.im);
    fp_set_zero(&P_jac->y.im);
    fp_set_zero(&P_jac->z.im);
}

void
point_jac_fp2_normalise(point_jac_fp2_t *P)
{
    fp2_inv(&P->z);
    fp2_mul(&P->x, &P->x, &P->z);
    fp2_mul(&P->x, &P->x, &P->z);
    fp2_mul(&P->y, &P->y, &P->z);
    fp2_mul(&P->y, &P->y, &P->z);
    fp2_mul(&P->y, &P->y, &P->z);
    fp2_set_one(&P->z);
}

void
point_jac_fp2_print(point_jac_fp2_t *P) {
    fp2_print("P.x ", &P->x);
    fp2_print("P.x ", &P->y);
    fp2_print("P.x ", &P->z);
}
