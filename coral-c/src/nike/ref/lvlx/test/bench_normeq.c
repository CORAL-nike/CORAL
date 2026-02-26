#include <nike.h>
#include <ec_params.h>
#include <stdio.h>
#include <bench_test_arguments.h>
#include <rng.h>
#include <bench.h>
#include <quaternion_data.h>
int
nike_bench_normeq(int bitsize, int iterations, int test, int single)
{
    int successes = 0, failures = 0, test_success = 1;
    clock_t time_start, time_end, time_total = 0;
    uint64_t cycles_start, cycles_end, cycles_total = 0;
    ibz_t p, x, y, a, two_e, sum, prod;
    int e = bitsize;
    ibz_init(&p);
    ibz_init(&x);
    ibz_init(&y);
    ibz_init(&a);
    ibz_init(&two_e);
    ibz_init(&sum);
    ibz_init(&prod);
    ibz_copy(&p, &(QUATALG_PINFTY.p));

    ibz_pow(&two_e, &ibz_const_two, 2 * e - 2);

    #ifdef FP2X_H
        char build_type[] = "broadwell";
    #else
        char build_type[] = "reference";
    #endif


    for (int i = 1; i < iterations + 1; i++) {
        time_start = clock();
        cycles_start = cpucycles();
        if (nike_normeq(&x, &y, &a, &p, &two_e)) {
            successes++;
        } else {
            failures++;
        }
        cycles_end = cpucycles();
        time_end = clock();
        cycles_total += cycles_end - cycles_start;
        time_total += time_end - time_start;
        if (test) {
            ibz_mul(&sum, &x, &x);
            ibz_mul(&prod, &a, &a);
            ibz_add(&sum, &sum, &prod);
            ibz_mul(&prod, &y, &y);
            ibz_mul(&prod, &prod, &p);
            ibz_add(&sum, &sum, &prod);
            test_success = test_success && (ibz_cmp(&sum, &two_e) == 0);
        }

        if (single) {
            printf(
                "\rNormeq Test #%5d/%d "
                "| log(p) = %4d, e = %4d (%s) "
                "| Success %5d "
                "| Avg: %7.2Lf ms / %7.2Lf MCy, This: %7.2Lf ms / %7.2Lf MCy\n",
                i,
                iterations,
                ibz_bitsize(&(QUATALG_PINFTY.p)),
                e,
                build_type,
                successes,
                (long double)time_total / CLOCKS_PER_SEC * 1000 / i,
                (long double)cycles_total / i / 1000000,
                (long double)(time_end - time_start) / CLOCKS_PER_SEC * 1000,
                (long double)(cycles_end - cycles_start) / 1000000
            );
        } else {
            printf(
                "\rNormeq Test #%5d/%d "
                "| log(p) = %4d, e = %4d (%s) "
                "| Success %5d "
                "| Avg: %7.2Lf ms / %7.2Lf MCy",
                i,
                iterations,
                ibz_bitsize(&(QUATALG_PINFTY.p)),
                e,
                build_type,
                successes,
                (long double)time_total / CLOCKS_PER_SEC * 1000 / i,
                (long double)cycles_total / i / 1000000
            );
            printf("   ");
            fflush(stdout);
        }
    }

    if (failures) {
        if (!single)
            printf("\n");
        printf("ERROR: `nike_normeq` failed at least once (non-ok return)\n");
    }

    if (test) {
        if (!single)
            printf("| ");

        if (test_success) {
            printf("All tests passed!\n");
        } else {
            printf("At least one test failed\n");
        }
    } else {
        printf("Output not tested for correctness. Use --test\n");
    }

    ibz_finalize(&p);
    ibz_finalize(&x);
    ibz_finalize(&y);
    ibz_finalize(&a);
    ibz_finalize(&two_e);
    ibz_finalize(&sum);
    ibz_finalize(&prod);
    return (!failures);
}

int
main(int argc, char *argv[])
{
    cpucycles_init();

    uint32_t seed[12] = { 0 };
    int iterations = 100;
    int bitsize = TORSION_EVEN_POWER;
    int test = 0;
    int single = 0;
    int help = 0;
    int seed_set = 0;

    for (int i = 1; i < argc; i++) {
        if (!help && strcmp(argv[i], "--help") == 0) {
            help = 1;
            continue;
        }

        if (!seed_set && !parse_seed(argv[i], seed)) {
            seed_set = 1;
            continue;
        }

        if (sscanf(argv[i], "--iterations=%d", &iterations) == 1) {
            continue;
        }
        if (sscanf(argv[i], "--bitsize=%d", &bitsize) == 1) {
            continue;
        }
        if (strcmp(argv[i], "--test") == 0) {
            test = 1;
            continue;
        }
        if (strcmp(argv[i], "--single") == 0) {
            single = 1;
            continue;
        }
    }
    if ((bitsize < (TORSION_EVEN_POWER - 1) / 2)) {
        help = 1;
    }

    if (help || iterations <= 0) {
        printf("Usage: %s [--iterations=<iterations>] [--bitsize=<bitsize>] [--seed=<seed>] [--test]\n", argv[0]);
        printf("Where <iterations> is the number of iterations used for testing; if not "
               "present, uses the default: %d)\n",
               iterations);
        printf("Where <bitsize> is the value of 1/2(exp+1); if not present, uses the default: %d)\n", bitsize);
        printf("Where <seed> is the random seed to be used; if not present, a random seed is "
               "generated\n");
        return 1;
    }

    if (!seed_set) {
        randombytes_select((unsigned char *)seed, sizeof(seed));
    }

    if (nike_bench_normeq(bitsize, iterations, test, single))
        return 0;

    return 1;
}
