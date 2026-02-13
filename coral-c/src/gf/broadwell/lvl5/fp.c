#include <assert.h>
#include "fp.h"

const digit_t p[NWORDS_FIELD] = { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                  0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x01afffffffffffff };
const digit_t p2[NWORDS_FIELD] = { 0xfffffffffffffffe, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
                                   0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x035fffffffffffff };

void
fp_sqrt(fp_t *x)
{
    fp_t tmp = *x;
    (void)gf27500_sqrt(x, &tmp);
}

uint32_t
fp_is_square(const fp_t *a)
{
    // ls is (0, 1, -1) and we want fp_is_square
    // to return 0xFF..FF when ls is 1 or 0 and 0x00..00 otherwise
    int32_t ls = gf27500_legendre(a);
    return ~(uint32_t)(ls >> 1);
}

void
fp_inv(fp_t *x)
{
    fp_t tmp = *x;
    (void)gf27500_invert(x, &tmp);
}

void
fp_div(fp_t *y, const fp_t *z, const fp_t *x)
{
    fp_t tmp;
    fp_copy(&tmp, x);
    fp_inv(&tmp);
    fp_mul(y, z, &tmp);
}

void
fp_exp3div4(fp_t *a)
{
    //
    // We optimise this by using the shape of the prime
    // to avoid almost all multiplications:
    //
    // We write:
    //     (p - 3) / 4 = (27*2^500 - 4) / 4
    //                 = 27*2^498 - 1
    //                 = 27*(2^498 - 1) + 26
    // Then we first compute:
    //     a498 = a**(2^498 - 1)
    // Then from this we get the desired result as:
    //     a**((p-3)/4) = a498**27 * a**26
    // We can compute this with 15 multiplications and 504 squares.
    fp_t z26, t3, t6, t9, tmp;
    // Compute a**3 and a**26
    fp_sqr(&z26, a);
    fp_mul(&tmp, a, &z26);
    fp_sqr(&z26, &z26);
    // Compute a**(2^3 - 1) = a**7
    fp_mul(&t3, &tmp, &z26);
    fp_sqr(&z26, &tmp);
    fp_sqr(&z26, &z26);
    fp_mul(&z26, &z26, a);
    fp_sqr(&z26, &z26);
    // Compute a**(2^6 - 1)
    fp_sqr(&t6, &t3);
    for (int i = 1; i < 3; i++)
        fp_sqr(&t6, &t6);
    fp_mul(&t6, &t6, &t3);
    // Compute a**(2^9 - 1)
    fp_sqr(&t9, &t6);
    for (int i = 1; i < 3; i++)
        fp_sqr(&t9, &t9);
    fp_mul(&t9, &t9, &t3);
    // Compute a**(2^15 - 1)
    fp_sqr(a, &t9);
    for (int i = 1; i < 6; i++)
        fp_sqr(a, a);
    fp_mul(a, a, &t6);
    // Compute a**(2^30 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 15; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^60 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 30; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^120 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 60; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^240 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 120; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(2^249 - 1)
    for (int i = 0; i < 9; i++)
        fp_sqr(a, a);
    fp_mul(a, a, &t9);
    // Compute a**(2^498 - 1)
    fp_sqr(&tmp, a);
    for (int i = 1; i < 249; i++)
        fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    // Compute a**(27*(2^498 - 1))
    fp_sqr(&tmp, a);
    fp_sqr(&tmp, &tmp);
    fp_sqr(&tmp, &tmp);
    fp_mul(a, a, &tmp);
    fp_sqr(&tmp, a);
    fp_mul(a, a, &tmp);
    // Compute a**(27*(2^498 - 1) + 26)
    fp_mul(a, a, &z26);
}

void
fp_batched_inv(fp_t *x, int len)
{
    fp_t t1[len], t2[len];
    fp_t inverse;

    // x = x0,...,xn
    // t1 = x0, x0*x1, ... ,x0 * x1 * ... * xn
    fp_copy(&t1[0], &x[0]);
    for (int i = 1; i < len; i++) {
        fp_mul(&t1[i], &t1[i - 1], &x[i]);
    }

    // inverse = 1/ (x0 * x1 * ... * xn)
    fp_copy(&inverse, &t1[len - 1]);
    fp_inv(&inverse);

    fp_copy(&t2[0], &inverse);
    // t2 = 1/ (x0 * x1 * ... * xn), 1/ (x0 * x1 * ... * x(n-1)) , ... , 1/xO
    for (int i = 1; i < len; i++) {
        fp_mul(&t2[i], &t2[i - 1], &x[len - i]);
    }

    fp_copy(&x[0], &t2[len - 1]);

    for (int i = 1; i < len; i++) {
        fp_mul(&x[i], &t1[i - 1], &t2[len - i - 1]);
    }
}
