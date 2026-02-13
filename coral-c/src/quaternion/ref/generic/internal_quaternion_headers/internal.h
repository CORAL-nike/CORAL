/** @file
 *
 * @authors Sina Schaeffler
 *
 * @brief Declarations for helper functions for quaternion algebra implementation
 */

#ifndef QUAT_HELPER_H
#define QUAT_HELPER_H

#include <quaternion.h>
#include <assert.h>
#include "intbig.h"

/** @internal
 * @ingroup quat_quat
 * @defgroup quat_helpers Quaternion module internal functions
 * @{
 */

/**  @internal
 * @defgroup quat_alg_helpers Helper functions for the alg library
 * @{
 */

/** @brief sqrt(-1) mod n for n= 1 mod 4
 *
 * Deterministic algorithm
 *
 * @returns 1 if such a sqrt was computed, 0 otherwise
 * @param sqrt Output: sqrt of -1 mod n if 1 is returned, any value otherwise
 * @param n Modulo, must be 1 mod 4. Success can be expected if it is prime (failure probability 1/(4(2**prime_length)))
 * @param primes primes used as tentative generators of (Z/nZ)*
 * @param primes_length length of primes
 */
int ibz_sqrt_minus_one(ibz_t *sqrt, const ibz_t *n, const uint32_t *primes, int primes_length);

/** @brief Attempt to solve x^2+y^2=p
 *
 * Deterministic algorithm using ibz_sqrt_minus_one
 *
 * @returns 1 if x and y were found, 0 otherwise
 * @param x Output
 * @param y Output
 * @param p Input, must be 1 mod 4. Success can be expected if it is prime (failure probability 1/(4(2**prime_length)))
 * @param primes primes used as tentative generators of (Z/nZ)*
 * @param primes_length length of primes
 */
int ibz_cornacchia_untested_prime(ibz_t *x, ibz_t *y, const ibz_t *p, const uint32_t *primes, int primes_length);

/** @brief Product of a and b in Z[sqrt(-q)]
 *
 * re_res + i*im_res = (re_a+i*im_a)*(re_b+i*im_b) where i a usual complex 4th root of unity
 *
 * @param re_res Output: real part of the result
 * @param im_res Output: imaginary part of the result
 * @param re_a Real part of a
 * @param im_a Imaginary part of a
 * @param re_b Real part of b
 * @param im_b Imaginary part of b
 * @param q Defined the ring Z[sart(-q)] in which multiplications are done.
 */
void ibz_complex_mul(ibz_t *re_res,
                     ibz_t *im_res,
                     const ibz_t *re_a,
                     const ibz_t *im_a,
                     const ibz_t *re_b,
                     const ibz_t *im_b,
                     const ibz_t *q);

/** @brief Multiplies res by a^e with res and a in Z[sqrt(-q)]
 *
 * re_res + i*im_res = (re_res+i*im_res)*((re_a+i*im_a)^exp) where i a root of -q
 *
 * @param re_res Output: real part of the result. Also used as input.
 * @param im_res Output: imaginary part of the result. Also used as input.
 * @param re_a Real part of a
 * @param im_a Imaginary part of a
 * @param q Defined the ring Z[sart(-q)] in which multiplications are done.
 * @param exp res*(a^exp) will be computed, exp should be a positive integer or 0
 */
void ibz_complex_mul_by_complex_power(ibz_t *re_res,
                                      ibz_t *im_res,
                                      const ibz_t *re_a,
                                      const ibz_t *im_a,
                                      const ibz_t *q,
                                      int64_t exp); // integers

/** @brief Multiplies to res the result of the solutions of cornacchia for prime depending on
 * valuation val (prime-adic valuation)
 *
 * re_res + i*im_res = (re_res+i*im_res)*((x+i*y)^val) where i a root of -q, and x,y an integer
 * sulotion to x^2 + qy^2 = prime
 *
 * @param re_res Output: real part of the result. Also used as input.
 * @param im_res Output: imaginary part of the result. Also used as input.
 * @param q Defined the ring Z[sart(-q)] in which multiplications are done.
 * @param prime a prime factor of n on which extended Cornacchia was called
 * @param val prime-adic valuation of the n on which extended Cornacchia was called
 * @returns 1 if an integer solution x,y to x^2 + y^2 = prime was found by Cornacchia_prime, 0
 * otherwise
 */
int ibz_cornacchia_extended_prime_loop(ibz_t *re_res,
                                       ibz_t *im_res,
                                       const ibz_t *q,
                                       int64_t prime,
                                       int64_t val); // integers

/**
 * @brief Find x and y such that x^2 + qy^2 = n
 *
 * Uses "extended" version of Cornacchia's algorithm which also allows to solve x^2 + y^2 = n for
 * some composite numbers. This uses a prime factor decomposition of n via trial division for primes
 * in the list, computes solutions for n's prime factors and then uses multiplication in
 * Z[sqrt(-q)]. Since for i^2=-q,(x+iz)(x-iy) = x^2 + qy^2, so a solution xa,ya for a and xb,yb for
 * b give a solution for ab by computing (xa+iya)*(xb+iyb).
 *
 * @param x Output
 * @param y Output
 * @param n parameter defining the equation. To get an output if one exists, only 1 of its prime
 * factors can exceed the largest prime in prime_list
 * @param params Inputs used for trial division by small numbers before applying Cornacchia if the remaining part is
 * prime. The field bad_primes_prod Assumed to be a product of small primes which are 3 mod 4. Used only to accelerate
 * failure in case its gcd with n is not 1. Can be NULL
 * @return 1 if success, 0 otherwise
 */
int ibz_cornacchia_extended(ibz_t *x, ibz_t *y, const ibz_t *n, const ibz_cornacchia_extended_params_t *params);

/** @}
 */
/** @}
 */

#endif
