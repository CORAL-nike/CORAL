#include <assert.h>
#include <time.h>

#include "ec.h"
#include "encoded_sizes.h"
#include "rng.h"
#include "bench.h"
#include "bench_test_arguments.h"

#include "nike.h"

uint32_t
parse_curve(curve_mg_fp_t *E) {
    curve_mg_fp_init(E);
    fp_t E_A;
    size_t n_parsed;
    if (!fp_fparse(&E_A, &n_parsed, 0, stdin) || n_parsed != 1) {
        printf(":: [DONE] Could not parse any more tests from from stdin.");
        return 0;
    }
    fp_copy(&E->A, &E_A);
    curve_mg_fp_normalise_A24(E);
    return 1;
}

int
main()
{
    clock_t time_start, time_end, time_total = 0;
    uint64_t cycles_start, cycles_end, cycles_total = 0;
    int success = 1, this_success;

    int i = 0;
    while (1) {
        i++;
        coral_secret_key_t sk;
        curve_mg_fp_t E_input, E_answer, E_computed;
        curve_mg_fp_init(&E_input);
        curve_mg_fp_init(&E_answer);
        curve_mg_fp_init(&E_computed);

        if (!coral_secret_key_parse(&sk)) break;
        if (!parse_curve(&E_input)) break;
        if (!parse_curve(&E_answer)) break;

        // fp_print("Computed Curve (before computation): ", &E_computed.A);

        time_start = clock();
        cycles_start = cpucycles();
        coral_compute_action(&E_computed, &sk, &E_input);
        cycles_end = cpucycles();
        time_end = clock();
        cycles_total += cycles_end - cycles_start;
        time_total += time_end - time_start;

        this_success = fp_is_equal(&E_computed.A, &E_answer.A);
        success = success | this_success;

        // printf("\n\n");
        printf("[%d] (Average) Action took: %" PRIu64 " cycles / %.3Lf ms %s (with e = %d)\n",
                i,
                cycles_total / i,
                (long double)time_total / CLOCKS_PER_SEC * 1000 / i,
                (this_success) ? "Success" : "Fail",
                sk.e
        );
        // fp_print("Answer Curve:   ", &E_answer.A);
        // fp_print("Computed Curve: ", &E_computed.A);

        coral_secret_key_finalize(&sk);
    }

    if (success) {
        printf("All tests passed\n");
    } else {
        printf("At least one test failed\n");
    }

    return 0;
}
