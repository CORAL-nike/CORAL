#include <assert.h>
#include <time.h>

#include "ec.h"
#include "encoded_sizes.h"
#include "rng.h"
#include "bench.h"
#include "bench_test_arguments.h"
#include "quaternion_data.h"

#include "nike.h"

uint32_t
parse_curve(curve_mg_fp_t *E) {
    curve_mg_fp_init(E);
    fp_t E_A;
    size_t n_parsed;
    if (!fp_fparse(&E_A, &n_parsed, 0, stdin) || n_parsed != 1) {
        return 0;
    }
    fp_copy(&E->A, &E_A);
    curve_mg_fp_normalise_A24(E);
    return 1;
}

int
main(int argc, char *argv[])
{
    #ifdef FP2X_H
        char build_type[] = "broadwell";
    #else
        char build_type[] = "reference";
    #endif

    cpucycles_init();

    int single = 0;
    int iterations = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--single") == 0) {
            single = 1;
            continue;
        }
        if (sscanf(argv[i], "--iterations=%d", &iterations) == 1) {
            continue;
        }
    }

    clock_t time_start, time_end, time_total = 0;
    uint64_t cycles_start, cycles_end, cycles_total = 0;
    int successes = 0, failures = 0;

    int i = 0;
    while (iterations == 0 || i < iterations) {
        i++;
        coral_secret_key_t sk;
        curve_mg_fp_t E_input, E_answer, E_computed;
        curve_mg_fp_init(&E_input);
        curve_mg_fp_init(&E_answer);
        curve_mg_fp_init(&E_computed);

        if (coral_secret_key_parse(&sk) != 1) break;
        if (!parse_curve(&E_input)) break;
        if (!parse_curve(&E_answer)) break;

        // fp_print("", &E_input.A);
        // fp_print("", &E_answer.A);

        // printf("\n\n\n");
        // fp_print("Input: ", &E_input.A);

        time_start = clock();
        cycles_start = cpucycles();
        coral_compute_action(&E_computed, &sk, &E_input);
        cycles_end = cpucycles();
        time_end = clock();
        cycles_total += cycles_end - cycles_start;
        time_total += time_end - time_start;

        if (fp_is_equal(&E_computed.A, &E_answer.A)) {
            successes++;
        } else {
            failures++;
        }

        if (single) {
            printf(
                "\rAction Test #%5d "
                "| log(p) = %4d, e = %4d (%s) "
                "| Success %5d "
                "| Avg: %7.2Lf ms / %7.2Lf MCy, This: %7.2Lf ms / %7.2Lf MCy\n",
                i,
                ibz_bitsize(&QUATALG_PINFTY.p),
                sk.e,
                build_type,
                successes,
                (long double)time_total / CLOCKS_PER_SEC * 1000 / i,
                (long double)cycles_total / i / 1000000,
                (long double)(time_end - time_start) / CLOCKS_PER_SEC * 1000,
                (long double)(cycles_end - cycles_start) / 1000000
            );
        } else {
            printf(
                "\rAction Test #%5d "
                "| log(p) = %4d, e = %4d (%s) "
                "| Success %5d "
                "| Avg: %7.2Lf ms / %7.2Lf Mcy",
                i,
                ibz_bitsize(&QUATALG_PINFTY.p),
                sk.e,
                build_type,
                successes,
                (long double)time_total / CLOCKS_PER_SEC * 1000 / i,
                (long double)cycles_total / i / 1000000
            );
            printf("   ");
            fflush(stdout);
        }

        // fp_print("Answer Curve:   ", &E_answer.A);
        // fp_print("Computed Curve: ", &E_computed.A);

        coral_secret_key_finalize(&sk);
    }

    if (!single)
        printf("| ");

    if (!failures) {
        printf("All tests passed!\n");
    } else {
        printf("At least one test failed\n");
    }

    return 0;
}
