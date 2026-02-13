#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "ec.h"
#include "ec_params.h"
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
    ibz_finalize(&sk->p);
}

uint32_t
coral_secret_key_parse(coral_secret_key_t *sk)
{
    coral_secret_key_init(sk);

    // Will be parsing p q x y e (Size at most p, encoded base10)
    // Will require at most FP_ENCODED_BYTES * log(16)/log(10)
    // Multiply by 2 for margin
    const uint32_t len = 5 * 2 * 2 * FP_ENCODED_BYTES;
    char decstring[len];

    if (fgets(decstring, len, stdin) == NULL) {
        printf(":: Nothing more to be read from stdin. Exiting.\n");
        coral_secret_key_finalize(sk);
        return 0;
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
