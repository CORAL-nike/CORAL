#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "ec.h"
#include "ec_params.h"
#include "e0_basis.h"
#include "encoded_sizes.h"
#include "inttypes.h"
#include "quaternion_constants.h"
#include "quaternion_data.h"
#include "quaternion.h"
#include "tools.h"

#include "nike.h"

void
coral_secret_key_init(coral_secret_key_t *sk)
{
    ibz_init(&sk->p);
    ibz_init(&sk->q);
    ibz_init(&sk->x);
    ibz_init(&sk->y);
}

void
coral_secret_key_finalize(coral_secret_key_t *sk)
{
    ibz_finalize(&sk->q);
    ibz_finalize(&sk->x);
    ibz_finalize(&sk->y);
}

void
coral_public_key_init(coral_public_key_t *pk)
{
    curve_mg_fp_init(&pk->curve);
}

int
coral_keygen(coral_secret_key_t *sk, coral_public_key_t *pk)
{
    coral_secret_key_init(sk);
    coral_public_key_init(pk);

    int success = 1;

    ibz_t a, two_pow_esub1, two_pow_2esub2;
    ibz_init(&two_pow_esub1);
    ibz_init(&two_pow_2esub2);
    ibz_init(&a);

    // :: Sample secret
    // Technically an (over) estimation of e, but within 1 of optimal value
    sk->e = ibz_bitsize(&QUATALG_PINFTY.p) / 2 + 128 + 11;
    ibz_pow(&two_pow_esub1, &ibz_const_two, sk->e - 1);
    ibz_mul(&two_pow_2esub2, &two_pow_esub1, &two_pow_esub1);

    success = success && nike_normeq(&sk->x, &sk->y, &a, &QUATALG_PINFTY.p, &two_pow_2esub2);

    // Recall: q = 2**(e - 1) - a
    ibz_sub(&sk->q, &two_pow_esub1, &a);
    ibz_finalize(&two_pow_esub1);
    ibz_finalize(&two_pow_2esub2);
    ibz_finalize(&a);

    // Compute various associated constants
    success = success && coral_secret_key_postinit(sk);

    // :: Compute public key
    // Copy necessary because scalar multiplication may normalise A24 (i.e. non-const)
    curve_mg_fp_t E0_copy;
    curve_mg_fp_init(&E0_copy);
    curve_mg_fp_copy(&E0_copy, &CURVE_E0);
    curve_mg_fp_normalise_and_A24(&E0_copy);
    success = success && coral_compute_action(&pk->curve, sk, &E0_copy);
    curve_mg_fp_normalise_and_A24(&pk->curve);

    return success;
}

int
coral_derive_secret(coral_shared_secret_t *shared, const coral_public_key_t *pk_B, const coral_secret_key_t *sk_A)
{
    // Copy necessary because scalar multiplication may normalise A24 (i.e. non-const)
    curve_mg_fp_t pk_B_copy;
    curve_mg_fp_init(&pk_B_copy);
    curve_mg_fp_copy(&pk_B_copy, &pk_B->curve);
    // Should already be normalised (public keys are normalised in keygen)
    pk_B_copy.is_A24_computed_and_normalized = false;
    curve_mg_fp_normalise_and_A24(&pk_B_copy);

    int res = coral_compute_action(&shared->curve, sk_A, &pk_B_copy);
    return res;
}

uint32_t
coral_secret_key_parse(coral_secret_key_t *sk)
{
    coral_secret_key_init(sk);

    // Will be parsing p q x y e (Size at most p, encoded base10)
    // Will require at most FP_ENCODED_BYTES * log(16)/log(10)
    // Multiply by 2 for margin
    #define len 4 * 2 * 2 * FP_ENCODED_BYTES
    char decstring[len];

    if (fgets(decstring, len, stdin) == NULL) {
        coral_secret_key_finalize(sk);
        return 2;
    }
    if (gmp_sscanf(decstring, "%Zd %Zd %Zd %Zd %d", &sk->p, &sk->q, &sk->x, &sk->y, &sk->e) != 5) {
        printf(":: [ERROR] Could not parse all of p, q, x, y, e from %s\n", decstring);
        coral_secret_key_finalize(sk);
        return 0;
    }

    // clock_t start = clock();
    coral_secret_key_postinit(sk);
    // printf("Postinit took %0.3Lf ms\n", (long double)(clock() - start) / CLOCKS_PER_SEC * 1000);
    return 1;
}
