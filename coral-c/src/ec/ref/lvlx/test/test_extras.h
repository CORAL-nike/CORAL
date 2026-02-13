
#ifndef TEST_EXTRAS_H
#define TEST_EXTRAS_H

#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <encoded_sizes.h>
#include <ec.h>
#include <fp.h>
#include <fp2.h>

#define PASSED 0
#define FAILED 1

// Generating a pseudo-random field element in [0, p-1]
void fp_random_test(fp_t *a);

// Generating a pseudo-random element in GF(p^2)
void fp2_random_test(fp2_t *a);

// Generating a random projective x-only point
void ec_random_test(xpoint_eqw_fp2_t *P, const curve_mg_fp2_t *curve);

// Generating a random projective x-only point and normalizing it
void ec_random_normalized_test(xpoint_eqw_fp2_t *P, const curve_mg_fp2_t *curve);

// Point difference
void projective_difference_point(xpoint_eqw_fp2_t *PQ, const xpoint_eqw_fp2_t *P, const xpoint_eqw_fp2_t *Q, const curve_mg_fp2_t *curve);

// _xpoint_eqw_fp2_dbl
void _xpoint_eqw_fp2_dbl(xpoint_eqw_fp2_t *Q, const xpoint_eqw_fp2_t *P, const xpoint_eqw_fp2_t *AC);

// Double-and-add
extern void _xpoint_eqw_fp2_dbladd(xpoint_eqw_fp2_t *R,
                    xpoint_eqw_fp2_t *S,
                    const xpoint_eqw_fp2_t *P,
                    const xpoint_eqw_fp2_t *Q,
                    const xpoint_eqw_fp2_t *PQ,
                    const xpoint_eqw_fp2_t *A24,
                    const bool A24_normalized);

#endif
