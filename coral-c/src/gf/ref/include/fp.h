#ifndef FP_H
#define FP_H

//////////////////////////////////////////////// NOTE: this is placed here for now
#include <sqisign_namespace.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <tutil.h>
#include <fp_constants.h>

typedef digit_t fp_t[NWORDS_FIELD]; // Datatype for representing field elements

extern const digit_t ONE[NWORDS_FIELD];
extern const digit_t ZERO[NWORDS_FIELD];
// extern const digit_t PM1O3[NWORDS_FIELD];

void fp_set_small(fp_t *x, const digit_t val);
void fp_mul_small(fp_t *x, const fp_t *a, const uint32_t val);
void fp_set_zero(fp_t *x);
void fp_set_one(fp_t *x);
uint32_t fp_is_equal(const fp_t *a, const fp_t *b);
uint32_t fp_is_zero(const fp_t *a);
void fp_copy(fp_t *out, const fp_t *a);

void fp_encode(void *dst, const fp_t *a);
void fp_decode_reduce(fp_t *d, const void *src, size_t len);
uint32_t fp_decode(fp_t *d, const void *src);

/** @brief Read fp element from file, encoded as in SQIsign signatures
 *
 * Parses space-delimited fp_t elements from first line of `*file`
 * Elements must be encoded as they are printed by `fp_print`, with the
 * exception that leading `0x` and/or `0` may be removed.
 *
 * Example: Assuming `FP_ENCODED_BYTES` is 4, all of
 * > `0001`
 * > `001`
 * > `01`
 * > `1`
 * are permitted (with or without the '0x' or '0X' prefixes)
 *
 * @param parsed: Array of parsed elements (must be allocated by caller)
 * @param n_parsed: Number of successfully parsed elements
 * @param parse: Number of elements to attempt to parse (0 means no limit)
 * @param file: File from which to read
 */
uint32_t fp_fparse(fp_t *parsed, size_t *n_parsed, size_t parse, FILE *file);
// Parse from string
uint32_t fp_parse(fp_t *parsed, char *hexstring, size_t len_hexstring);

void fp_select(fp_t *d, const fp_t *a0, const fp_t *a1, uint32_t ctl);
void fp_cswap(fp_t *a, fp_t *b, uint32_t ctl);

void fp_add(fp_t *out, const fp_t *a, const fp_t *b);
void fp_sub(fp_t *out, const fp_t *a, const fp_t *b);
void fp_neg(fp_t *out, const fp_t *a);
void fp_sqr(fp_t *out, const fp_t *a);
void fp_mul(fp_t *out, const fp_t *a, const fp_t *b);

void fp_inv(fp_t *x);
uint32_t fp_is_square(const fp_t *a);
void fp_sqrt(fp_t *a);
void fp_half(fp_t *out, const fp_t *a);
void fp_exp3div4(fp_t *out, const fp_t *a);
void fp_div3(fp_t *out, const fp_t *a);

void fp_print(const char *name, const fp_t *a);
uint32_t fp_is_one(const fp_t *a);

void fp_add_one(fp_t *x, const fp_t *y);
void fp_div(fp_t *x, const fp_t *y, const fp_t *z);
void fp_batched_inv(fp_t *x, int len);

#ifndef NDEBUG
    extern uint32_t N_FP_MUL;
    extern uint32_t N_FP_SQR;
#endif

#endif
