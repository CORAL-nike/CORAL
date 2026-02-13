/** @file
 *
 * @authors Luca De Feo, Sina Schaeffler
 *
 * @brief Declarations for quaternion algebra operations
 */

#ifndef QUATERNION_H
#define QUATERNION_H

// #include <rng.h>
#include <sqisign_namespace.h>
#include "intbig.h"
#include <assert.h>

/** @defgroup quat_quat Quaternion algebra
 * @{
 */

/** @defgroup quat_vec_t Types for integer vectors and matrices
 * @{
 */

/** @brief Type for vector of 2 big integers
 *
 * @typedef ibz_vec_2_t
 */
typedef ibz_t ibz_vec_2_t[2];

/** @brief Type for vectors of 4 integers
 *
 * @typedef ibz_vec_4_t
 *
 * Represented as a vector of 4 ibz_t (big integer) elements
 */
typedef ibz_t ibz_vec_4_t[4];

/** @brief Type for 2 by 2 matrices of integers
 *
 * @typedef ibz_mat_2x2_t
 *
 * Represented as a matrix of 2 vectors of 2 ibz_t (big integer) elements
 */
typedef ibz_t ibz_mat_2x2_t[2][2];

/** @brief Type for 4 by 4 matrices of integers
 *
 * @typedef ibz_mat_4x4_t
 *
 * Represented as a matrix of 4 vectors of 4 ibz_t (big integer) elements
 */
typedef ibz_t ibz_mat_4x4_t[4][4];
/**
 * @}
 */

/** @defgroup quat_quat_t Types for quaternion algebras
 * @{
 */

/** @brief Type for quaternion algebras
 *
 * @typedef quat_alg_t
 *
 * @struct quat_alg
 *
 * The quaternion algebra ramified at p = 3 mod 4 and ∞.
 */
typedef struct quat_alg
{
    ibz_t p; ///< Prime number, must be = 3 mod 4.
} quat_alg_t;

/** @brief Type for quaternion algebra elements
 *
 * @typedef quat_alg_elem_t
 *
 * @struct quat_alg_elem
 *
 * Represented as a array *coord* of 4 ibz_t integers and a common ibz_t denominator *denom*.
 *
 * The representation is not necessarily normalized, that is, gcd(denom, content(coord)) might not
 * be 1. For getting a normalized representation, use the quat_alg_normalize function
 *
 * The elements are always represented in basis (1,i,j,ij) of the quaternion algebra, with i^2=-1
 * and j^2 = -p
 */
typedef struct quat_alg_elem
{
    ibz_t denom;       ///< Denominator by which all coordinates are divided (big integer, must not be 0)
    ibz_vec_4_t coord; ///< Numerators of the 4 coordinates of the quaternion algebra element in basis (1,i,j,ij)
} quat_alg_elem_t;

/** @brief Type for lattices in dimension 4
 *
 * @typedef quat_lattice_t
 *
 * @struct quat_lattice
 *
 * Represented as a rational (`frac`) times an integreal lattice (`basis`)
 *
 * The basis is such that its columns divided by its denominator are elements of
 * the quaternion algebra, represented in basis (1,i,j,ij) where i^2 = -1, j^2 = -p.
 *
 * All lattices must have full rank (4)
 */
typedef struct quat_lattice
{
    ibz_t denom;         ///< Denominator by which the basis is divided (big integer, must not be 0)
    ibz_mat_4x4_t basis; ///< Integer basis of the lattice  (its columns divided by denom are
                         ///< algebra elements in the usual basis)
} quat_lattice_t;

/** @brief Type for left ideals of maximal orders in quaternion algebras
 *
 * @typedef quat_left_ideal_t
 *
 * @struct quat_left_ideal
 *
 * The basis of the lattice representing it is such that its columns divided by its denominator are
 * elements of the quaternion algebra, represented in basis (1,i,j,ij) where i^2 = -1, j^2 = -p.
 */
typedef struct quat_left_ideal
{
    quat_lattice_t lattice;             ///< lattice representing the ideal
    ibz_t norm;                         ///< norm of the lattice
    const quat_lattice_t *parent_order; ///< should be a maximal order
} quat_left_ideal_t;
/** @}
 */

/** @brief Type for extremal maximal orders
 *
 * @typedef quat_p_extremal_maximal_order_t
 *
 * @struct quat_p_extremal_maximal_order
 *
 * The basis of the order representing it is in hermite normal form, and its columns divid
ed by its denominator are elements of the quaternion algebra, represented in basis (1,z,t,
tz) where z^2 = -q, t^2 = -p.
*/
typedef struct quat_p_extremal_maximal_order
{
    quat_lattice_t order; ///< the order represented as a lattice
    quat_alg_elem_t z;    ///< the element of small discriminant
    quat_alg_elem_t t;    ///< the element of norm p orthogonal to z
    uint32_t q;           ///< the absolute value of the square of z
} quat_p_extremal_maximal_order_t;

/** @brief Type for represent integer parameters
 *
 * @typedef quat_represent_integer_params_t
 *
 * @struct quat_represent_integer_params
 *
 */
typedef struct quat_represent_integer_params
{
    int primality_test_iterations;                ///< Primality test iterations
    const quat_p_extremal_maximal_order_t *order; ///< The standard extremal maximal order
    const quat_alg_t *algebra;                    ///< The quaternion algebra
} quat_represent_integer_params_t;

/** @brief Type for extended cornacchia parameters
 *
 * @typedef ibz_cornacchia_extended_params_t
 *
 * @struct ibz_cornacchia_extended_params
 *
 */
typedef struct ibz_cornacchia_extended_params
{
    int primality_test_iterations; ///< Primality test iterations
    int q;                         ///< The equation to be solved is of the form x^2+qy^2=m
    const short *prime_list;       ///< List of small primes p such that -q is a square mod p
    unsigned prime_list_length;    ///< Lengt of prime_list
    const ibz_t *bad_primes_prod;  ///< Product of small primes p such that -q is not a square mod p
} ibz_cornacchia_extended_params_t;

/** @brief Type for extended cornacchia trial division constants
 *
 * @typedef ibz_cornacchia_trial_division_consts_t
 *
 * @struct ibz_cornacchia_trial_division_consts
 *
 */
typedef struct ibz_cornacchia_trial_division_consts
{
    int primality_test_iterations; ///< Primality test iterations
    const uint32_t *good_primes;   ///< List of small 1mod4 primes p
    const uint32_t *bad_primes;    ///< List of small 3mod4 primes p
    int good_primes_length;        ///< Length of good_primes
    int bad_primes_length;         ///< Length of bad_primes

} ibz_cornacchia_trial_division_consts_t;

/** @brief Type for extended cornacchia trial division output
 *
 * @typedef ibz_cornacchia_trial_division_output_t
 *
 * @struct ibz_cornacchia_trial_division_output
 *
 */
typedef struct ibz_cornacchia_trial_division_output
{
    uint16_t *good_half_exponents; ///< Length of good_primes
    uint16_t *bad_half_exponents;  ///< Length of bad_primes
    char *good_remainders;         ///< Length of bad_primes

} ibz_cornacchia_trial_division_output_t;

/*************************** Functions *****************************/

/** @defgroup quat_c Constructors and Destructors
 * @{
 */
void quat_alg_init_set(quat_alg_t *alg, const ibz_t *p);
void quat_alg_finalize(quat_alg_t *alg);

void quat_alg_elem_init(quat_alg_elem_t *elem);
void quat_alg_elem_finalize(quat_alg_elem_t *elem);

void ibz_vec_2_init(ibz_vec_2_t *vec);
void ibz_vec_2_finalize(ibz_vec_2_t *vec);

void ibz_vec_4_init(ibz_vec_4_t *vec);
void ibz_vec_4_finalize(ibz_vec_4_t *vec);

void ibz_mat_2x2_init(ibz_mat_2x2_t *mat);
void ibz_mat_2x2_finalize(ibz_mat_2x2_t *mat);

void ibz_mat_4x4_init(ibz_mat_4x4_t *mat);
void ibz_mat_4x4_finalize(ibz_mat_4x4_t *mat);

void quat_lattice_init(quat_lattice_t *lat);
void quat_lattice_finalize(quat_lattice_t *lat);

void quat_left_ideal_init(quat_left_ideal_t *lideal);
void quat_left_ideal_finalize(quat_left_ideal_t *lideal);
/** @}
 */

/** @defgroup quat_printers Print functions for types from the quaternion module
 * @{
 */
void ibz_mat_2x2_print(const ibz_mat_2x2_t *mat);
void ibz_mat_4x4_print(const ibz_mat_4x4_t *mat);
void ibz_vec_2_print(const ibz_vec_2_t *vec);
void ibz_vec_4_print(const ibz_vec_4_t *vec);

void quat_lattice_print(const quat_lattice_t *lat);
void quat_alg_print(const quat_alg_t *alg);
void quat_alg_elem_print(const quat_alg_elem_t *elem);
void quat_left_ideal_print(const quat_left_ideal_t *lideal);

/** @}
 */

/** @defgroup quat_int Integer functions for quaternion algebra
 * @{
 */

/**
 * @brief Generates a random prime
 *
 * A number is accepted as prime if it passes a 30-round Miller-Rabin test.
 * This function is fairly inefficient and mostly meant for tests.
 *
 * @returns 1 if a prime is found, 0 otherwise
 * @param p Output: The prime (if found)
 * @param is3mod4 If 1, the prime is required to be 3 mod 4, if 0 no congruence condition is imposed
 * @param bitsize Maximal size of output prime
 * @param probability_test_iterations Miller-Rabin iteartions for probabilistic primality testing in
 * rejection sampling
 */
int ibz_generate_random_prime(ibz_t *p, int is3mod4, int bitsize, int probability_test_iterations);

/**
 * @brief Find integers x and y such that x^2 + n*y^2 = p
 *
 * Uses Cornacchia's algorithm, should be used  only for prime p
 *
 * @param x Output
 * @param y Output
 * @param n first parameter defining the equation
 * @param p seond parameter defining the equation, must be prime
 * @return 1 if success, 0 otherwise
 */
int ibz_cornacchia_prime(ibz_t *x, ibz_t *y, const ibz_t *n, const ibz_t *p);

/**
 * @brief Find integers x and y such that x^2 + n*y^2 = n
 *
 * Uses Cornacchia's algorithm and trial division. Only for even numbers. Can fail
 *
 * @param x Output
 * @param y Output
 * @param factorization Memory for computation. Vectors must be of the size good_primes_length and bad_primes_length
 * given in consts
 * @param n parameter defining the equation. Must be odd and positive
 * @param consts Constants used in the trial division phase of the algorithm
 * @return 1 if success, 0 otherwise
 */
int ibz_cornacchia_extended_trials(ibz_t *x,
                                   ibz_t *y,
                                   ibz_cornacchia_trial_division_output_t *factorization,
                                   const ibz_t *n,
                                   const ibz_cornacchia_trial_division_consts_t *consts);
// quat_int
/** @}
 */
// end quat_quat
/** @}
 */

#endif
