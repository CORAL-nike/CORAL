/** @file
 *
 * @authors Sina Schaeffler
 *
 * @brief Declarations of tests of quaternion algebra operations
 */

#ifndef QUATERNION_TESTS_H
#define QUATERNION_TESTS_H

#include <quaternion.h>
#include <stdio.h>
#include "internal.h"

/** @internal
 * @ingroup quat_helpers
 * @defgroup quat_tests Quaternion module test functions
 * @{
 */


/** @brief Test for integer functions
 *
 * void ibz_init(ibz_t *x);
 *
 * void ibz_finalize(ibz_t *x);
 *
 * void ibz_add(ibz_t *sum, const ibz_t *a, const ibz_t *b);
 *
 * void ibz_sub(ibz_t *diff, const ibz_t *a, const ibz_t *b);
 *
 * void ibz_mul(ibz_t *prod, const ibz_t *a, const ibz_t *b);
 *
 * void ibz_neg(ibz_t *neg, const ibz_t *a);
 *
 * void ibz_abs(ibz_t *abs, const ibz_t *a);
 *
 * void ibz_div(ibz_t *quotient, ibz_t *remainder, const ibz_t *a, const ibz_t *b);
 *
 * void ibz_div_2exp(ibz_t *quotient, const ibz_t *a, uint32_t exp);
 *
 * void ibz_mod(ibz_t *r, const ibz_t *a, const ibz_t *b);
 *
 * unsigned long int ibz_mod_ui(const mpz_t *n, unsigned long int d);
 *
 * int ibz_divides(const ibz_t *a, const ibz_t *b);
 *
 * void ibz_pow(ibz_t *pow, const ibz_t *x, uint32_t e);
 *
 * void ibz_pow_mod(ibz_t *pow, const ibz_t *x, const ibz_t *e, const ibz_t *m);
 *
 * int ibz_cmp(const ibz_t *a, const ibz_t *b);
 *
 * int ibz_is_zero(const ibz_t *x);
 *
 * int ibz_is_one(const ibz_t *x);
 *
 * int ibz_cmp_int32(const ibz_t *x, int32_t y);
 *
 * int ibz_is_even(const ibz_t *x);
 *
 * int ibz_is_odd(const ibz_t *x);
 *
 * void ibz_set(ibz_t *i, int32_t x);
 *
 * void ibz_copy(ibz_t *target, const ibz_t *value);
 *
 * void ibz_swap(ibz_t *a, ibz_t *b);
 *
 * void ibz_copy_digits(ibz_t *target, const digit_t *dig, int dig_len);
 *
 * void ibz_to_digits(digit_t *target, const ibz_t *ibz);
 *
 * int32_t ibz_get(const ibz_t *i);
 *
 * int ibz_rand_interval(ibz_t *rand, const ibz_t *a, const ibz_t *b);
 *
 * int ibz_rand_interval_minm_m(ibz_t *rand, int32_t m);
 *
 * int ibz_bitsize(const ibz_t *a);
 *
 * void ibz_gcd(ibz_t *gcd, const ibz_t *a, const ibz_t *b);
 *
 * int ibz_invmod(ibz_t *inv, const ibz_t *a, const ibz_t *mod);
 *
 * void ibz_sqrt_floor(ibz_t *sqrt, const ibz_t *a);
 */
int ibz_test_intbig(void);

/** @brief Test for implementations of GMP functions missing from the mini-GMP API
 *
 * int mpz_legendre(const mpz_t a, const mpz_t p);
 *
 * double mpz_get_d_2exp(signed long int *exp, const mpz_t op);
 */
int mini_gmp_test(void);

/** @brief Test initializers and finalizers for quaternion algebra types
 *
 * Test initializers and finalizers for the following types:
 *
 * quat_alg_t
 *
 * quat_alg_elem_t
 *
 * quat_alg_coord_t
 *
 * ibz_vec_2_t
 *
 * ibz_vec_4_t
 *
 * ibz_mat_2x2_t
 *
 * ibz_mat_4x4_t
 *
 * quat_lattice_t
 *
 * quat_lattice_t
 *
 * quat_left_ideal_t
 */
int quat_test_finit(void);

/** @brief Test integer functions
 *
 * Runs unit tests for the following functions
 *
 * int ibz_generate_random_prime(ibz_t *p, int is3mod4, int bitsize);
 *
 * int ibz_cornacchia_prime(ibz_t *x, ibz_t *y, const ibz_t *n, const ibz_t *p);
 */
int quat_test_integers(void);

/** @brief Test with randomization for complex functions where this is possible
 *
 */
int quat_test_with_randomization(void);

/** @}
 */

#endif
