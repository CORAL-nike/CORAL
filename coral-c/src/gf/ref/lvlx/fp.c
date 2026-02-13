#include <stdio.h>

#include "encoded_sizes.h"
#include "fp.h"

#ifndef NDEBUG
    uint32_t N_FP_MUL;
    uint32_t N_FP_SQR;
#endif

/*
 * If ctl == 0x00000000, then *d is set to a0
 * If ctl == 0xFFFFFFFF, then *d is set to a1
 * ctl MUST be either 0x00000000 or 0xFFFFFFFF.
 */
void
fp_select(fp_t *d, const fp_t *a0, const fp_t *a1, uint32_t ctl)
{
    digit_t cw = (int32_t)ctl;
    for (unsigned int i = 0; i < NWORDS_FIELD; i++) {
        (*d)[i] = (*a0)[i] ^ (cw & ((*a0)[i] ^ (*a1)[i]));
    }
}

void
fp_print(const char *name, const fp_t *a)
{
    printf("%s0x", name);

    uint8_t buf[FP_ENCODED_BYTES];
    fp_encode(&buf, a); // Encoding ensures canonical rep
    for (int i = 0; i < FP_ENCODED_BYTES; i++) {
        printf("%02x", buf[FP_ENCODED_BYTES - i - 1]);
    }
    printf("\n");
}

// Is a GF(p^2) element one?
// Returns 0xFF...FF (true) if a=1, 0 (false) otherwise
uint32_t
fp_is_one(const fp_t *a)
{
    return fp_is_equal(a, &ONE);
}

void
fp_div(fp_t *x, const fp_t *y, const fp_t *z)
{
    fp_t z_inv;
    fp_copy(&z_inv, z);
    fp_inv(&z_inv);
    fp_mul(x, y, &z_inv);
}

void
fp_add_one(fp_t *x, const fp_t *y)
{
    fp_add(x, y, &ONE);
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
