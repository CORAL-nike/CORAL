/** @file
 *
 * @brief The key generation and signature protocols
 */

#ifndef NIKE_H
#define NIKE_H

#include "sqisign_namespace.h"

#include "ec.h"
#include "hd.h"
#include "quaternion.h"

/** @defgroup nike NIKE tools and functions
 * @{
 */
/** @defgroup nike_t Types for NIKE protocol
 * @{
 */

/** @brief Secret key type
 *
 * @typedef coral_secret_key_t
 * @struct coral_secret_key
 *
 */
typedef struct coral_secret_key
{
    ibz_t p, q, x, y;
    uint32_t e;

    // Computed in postinit as a function of p, q, x, y, e
    digit_t P1_mult[NWORDS_FIELD], P2_mult[NWORDS_FIELD], Q1_mult[NWORDS_FIELD], Q2_mult[NWORDS_FIELD];
    uint32_t P1_mult_bits, P2_mult_bits, Q1_mult_bits, Q2_mult_bits;
    uint32_t P1_neg, P2_neg, Q1_neg, Q2_neg;
    uint32_t v, s_v2, t_v2, postinit;
} coral_secret_key_t;

/** @brief Public key type
 *
 * @typedef coral_public_key_t
 * @struct coral_public_key
 *
 * Athough a public key is usually just a curve, but by wrapping like this, we
 * give ourselves room to append associated data in the future (e.g. basis
 * hints).
 */
typedef struct coral_public_key_t
{
    curve_mg_fp2_t curve;
} coral_public_key_t;

/** @}
 */

/** @defgroup nike_helper Auxiliary functions for the NIKE
 * @{
 */

void coral_secret_key_init(coral_secret_key_t *sk);
void coral_secret_key_postinit(coral_secret_key_t *sk);
void coral_secret_key_finalize(coral_secret_key_t *sk);
void coral_secret_key_postinit(coral_secret_key_t *sk);

void
coral_prepare_hd_kernel(
        theta_couple_jac_point_fp2_t *C1_jac_dbls,
        theta_couple_jac_point_fp2_t *C2_jac_dbls,
        theta_couple_curve_fp_t *E12,
        uint16_t *strategy,
        uint16_t strategy_length,
        const coral_secret_key_t *sk,
        point_eqw_fp_t *P,
        curve_mg_fp_t *E,
        point_eqw_fp_t *Q,
        curve_mg_fp_t *Et
);

uint32_t
coral_compute_hd(
    theta_couple_curve_fp_t *E34,
    theta_couple_curve_fp_t *E12,
    uint32_t chain_length,
    theta_couple_jac_point_fp2_t *C1_jac_dbls,
    theta_couple_jac_point_fp2_t *C2_jac_dbls,
    uint16_t *strategy,
    uint16_t strategy_length
);

uint32_t
coral_compute_action(
    curve_mg_fp_t *E_action,
    const coral_secret_key_t *sk,
    curve_mg_fp_t *E
);

/**
 * @brief Parse secret key from `stdin`
 * @param sk Output: a secret key
 * @returns 1 on success, 0 otherwise
 */
uint32_t coral_secret_key_parse(coral_secret_key_t *sk);

int nike_normeq(ibz_t *x, ibz_t *y, ibz_t *a, const ibz_t *p, const ibz_t *two_e);

/** @}
 */

/** @defgroup coral_protocols Protocols of a CORAL key-exchange
 * @{
 */

/**
 * @brief Key generation
 * @param sk Output: a (randomly generated) secret key
 * @param pk Output: the public key corresponding to `sk`
 * @returns 1 on success, 0 otherwise
 */
int coral_keygen(coral_secret_key_t *sk, coral_public_key_t *pk);

/**
 * @brief Derive shared secret
 * @param shared Output: the shared secret corresponding to `sk`, `pk`
 * @param pk Input: a public key
 * @param sk Input: a secret key
 * @returns 1 on success, 0 otherwise
 */
int coral_derive_secret(coral_public_key_t *shared, coral_public_key_t *pk, coral_secret_key_t *sk);

/** @}
 */

/** @}
 */

#endif
