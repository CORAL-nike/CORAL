#include <nike.h>
#include <ec_params.h>
#include <stdio.h>
#include <bench_test_arguments.h>
#include <rng.h>
#include <bench.h>
#include <quaternion_data.h>
int
nike_bench_normeq(int bitsize, int iterations, int test)
{
    int res = 0;
    uint64_t start, end;
    uint64_t sumc = 0;
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
    ibz_printf("%d\n", ibz_bitsize(&(QUATALG_PINFTY.p)));

    ibz_pow(&two_e, &ibz_const_two, 2 * e - 2);

    for (int i = 0; i < iterations; i++) {
        start = cpucycles();
        res = res | !nike_normeq(&x, &y, &a, &p, &two_e);
        end = cpucycles();
        sumc = sumc + end - start;
        if (test) {
            ibz_mul(&sum, &x, &x);
            ibz_mul(&prod, &a, &a);
            ibz_add(&sum, &sum, &prod);
            ibz_mul(&prod, &y, &y);
            ibz_mul(&prod, &prod, &p);
            ibz_add(&sum, &sum, &prod);
            res = res | !(ibz_cmp(&sum, &two_e) == 0);
        }
    }
    printf("Normeq solving took: %" PRIu64 " cycles on average per iteration (adjusted bitsize e=%d)\n",
           sumc / iterations,
           e);
    if (test && res) {
        printf("NIKE tool test normeq_randomized failed\n");
    }
    ibz_finalize(&p);
    ibz_finalize(&x);
    ibz_finalize(&y);
    ibz_finalize(&a);
    ibz_finalize(&two_e);
    ibz_finalize(&sum);
    ibz_finalize(&prod);
    return (res);
}

int
main(int argc, char *argv[])
{
    uint32_t seed[12] = { 0 };
    int iterations = 100;
    int bitsize = TORSION_EVEN_POWER;
    int test = 0;
    int help = 0;
    int seed_set = 0;
    int res = 0;

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
        if (sscanf(argv[i], "--test")) {
            test = 1;
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
        printf("Where <test> enables testing all outputs produced (invalidating the benchmarks)\n");
        return 1;
    }

    if (!seed_set) {
        randombytes_select((unsigned char *)seed, sizeof(seed));
    }

    printf("--------------------------------------------------------------------------------\n\n");
    printf("Benchmarking normeq: e %d, iterations: %d, test: %d\n\n", bitsize, iterations, test);
    print_seed(seed);

    res = res | nike_bench_normeq(bitsize, iterations, test);

    return res;
}
