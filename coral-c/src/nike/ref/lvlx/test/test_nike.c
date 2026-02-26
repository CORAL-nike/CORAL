#include <stdio.h>
#include <inttypes.h>

#include "bench.h"
#include "quaternion_data.h"
#include "nike.h"

int
testbench_nike(int iterations, int single)
{
    int keygen_success, keygen_successes = 0;
    int secret_derivation_success, secret_derivation_successes = 0;
    int exchange_successes = 0;

    clock_t keygen_time_start, keygen_time_end, keygen_time_total = 0;
    uint64_t keygen_cycles_start, keygen_cycles_end, keygen_cycles_total = 0;

    clock_t secret_derivation_time_start, secret_derivation_time_end, secret_derivation_time_total = 0;
    uint64_t secret_derivation_cycles_start, secret_derivation_cycles_end, secret_derivation_cycles_total = 0;

    coral_secret_key_t sk_A, sk_B;
    coral_public_key_t pk_A, pk_B;
    coral_shared_secret_t shared_A, shared_B;

    #ifdef FP2X_H
        char build_type[] = "broadwell";
    #else
        char build_type[] = "reference";
    #endif

    for (int i = 1; i < iterations + 1; i++) {
        keygen_time_start = clock();
        keygen_cycles_start = cpucycles();
            keygen_success = coral_keygen(&sk_A, &pk_A);
            keygen_success = keygen_success && coral_keygen(&sk_B, &pk_B);
        keygen_cycles_end = cpucycles();
        keygen_time_end = clock();
        if (keygen_success)
            keygen_successes++;

        assert(!fp_is_equal(&pk_A.curve.A, &pk_B.curve.A));

        keygen_cycles_total += keygen_cycles_end - keygen_cycles_start;
        keygen_time_total += keygen_time_end - keygen_time_start;

        secret_derivation_time_start = clock();
        secret_derivation_cycles_start = cpucycles();
            secret_derivation_success = coral_derive_secret(&shared_A, &pk_B, &sk_A);
            secret_derivation_success = secret_derivation_success && coral_derive_secret(&shared_B, &pk_A, &sk_B);
        secret_derivation_cycles_end = cpucycles();
        secret_derivation_time_end = clock();
        if (secret_derivation_success)
            secret_derivation_successes++;

        secret_derivation_cycles_total += secret_derivation_cycles_end - secret_derivation_cycles_start;
        secret_derivation_time_total += secret_derivation_time_end - secret_derivation_time_start;

        if (keygen_success && secret_derivation_success && fp_is_equal(&shared_A.curve.A, &shared_B.curve.A))
            exchange_successes++;

        assert(sk_A.e == sk_B.e);


        if (single) {
            printf(
                "\rNike Test #%5d/%d "
                "| log(p) = %4d, e = %4d (%s) "
                "| Success %5d "
                "| [Keygen] Avg: %7.2Lf ms / %7.2Lf MCy, This: %7.2Lf ms / %7.2Lf MCy "
                "| [SharedKey] Avg: %7.2Lf ms / %7.2Lf MCy, This: %7.2Lf ms / %7.2Lf MCy\n",
                i,
                iterations,
                ibz_bitsize(&(QUATALG_PINFTY.p)),
                sk_A.e,
                build_type,
                exchange_successes,
                (long double)keygen_time_total / CLOCKS_PER_SEC * 1000 / i / 2,
                (long double)keygen_cycles_total / i / 2 / 1000000,
                (long double)(keygen_time_end - keygen_time_start) / CLOCKS_PER_SEC * 1000 / 2,
                (long double)(keygen_cycles_end - keygen_cycles_start) / 2 / 1000000,
                (long double)secret_derivation_time_total / CLOCKS_PER_SEC * 1000 / i / 2,
                (long double)secret_derivation_cycles_total / i / 2 / 1000000,
                (long double)(secret_derivation_time_end - secret_derivation_time_start) / CLOCKS_PER_SEC * 1000 / 2,
                (long double)(secret_derivation_cycles_end - secret_derivation_cycles_start) / 2 / 1000000
            );
        } else {
            printf(
                "\rNike Test #%5d/%d "
                "| log(p) = %4d, e = %4d (%s) "
                "| Success %5d "
                "| [Keygen] Avg: %7.2Lf ms / %8.2Lf MCy "
                "| [SharedKey] Avg: %7.2Lf ms / %7.2Lf MCy",
                i,
                iterations,
                ibz_bitsize(&(QUATALG_PINFTY.p)),
                sk_A.e,
                build_type,
                exchange_successes,
                (long double)keygen_time_total / CLOCKS_PER_SEC * 1000 / i / 2,
                (long double)keygen_cycles_total / i / 2 / 1000000,
                (long double)secret_derivation_time_total / CLOCKS_PER_SEC * 1000 / i / 2,
                (long double)secret_derivation_cycles_total / i / 2 / 1000000
            );
            printf("   ");
            fflush(stdout);
        }
    }


    if (exchange_successes < iterations) {
        if (!single)
            printf("\n");

        printf("[Error]: At least one exchange failed because (shared secret did not match)\n");

        if (keygen_successes < iterations) {
            printf("=> Only %d/%d keygen succeeded\n", keygen_successes, iterations);
        } else {
            printf("=> All keygens succeeded\n");
        }

        if (secret_derivation_successes < iterations) {
            printf("=> Only %d/%d individual secret derivation succeeded\n", secret_derivation_successes, iterations);
        } else {
            printf("=> All individual secret derivations succeeded\n");
        }

    } else {
        if (!single)
            printf("| ");

        printf("All exchanges succeeded!\n");
    }


    coral_secret_key_finalize(&sk_A);
    coral_secret_key_finalize(&sk_B);

    return (exchange_successes == iterations);
}

int
main(int argc, char *argv[])
{
    int iterations = 100;
    int single = 0;
    cpucycles_init();

    for (int i = 1; i < argc; i++) {
        if (sscanf(argv[i], "--iterations=%d", &iterations) == 1) {
            continue;
        }
        if (strcmp(argv[i], "--single") == 0) {
            single = 1;
            continue;
        }
    }

    if (testbench_nike(iterations, single))
        return 0;

    return 1;
}
