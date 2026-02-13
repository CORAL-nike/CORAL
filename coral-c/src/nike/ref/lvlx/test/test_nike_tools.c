#include <nike.h>
#include <stdio.h>
#include <bench_test_arguments.h>
#include <rng.h>

int
nike_test_normeq_static()
{
    int res = 0;
    ibz_t p, x, y, a, two_e, sum, prod;
    ibz_init(&p);
    ibz_init(&x);
    ibz_init(&y);
    ibz_init(&a);
    ibz_init(&two_e);
    ibz_init(&sum);
    ibz_init(&prod);
    ibz_set(&p, 7823);
    ibz_pow(&two_e, &ibz_const_two, 24);
    res = res | !nike_normeq(&x, &y, &a, &p, &two_e);
    ibz_mul(&sum, &x, &x);
    ibz_mul(&prod, &a, &a);
    ibz_add(&sum, &sum, &prod);
    ibz_mul(&prod, &y, &y);
    ibz_mul(&prod, &prod, &p);
    ibz_add(&sum, &sum, &prod);
    res = res | !(ibz_cmp(&sum, &two_e) == 0);
    if (res) {
        printf("NIKE tool test normeq_static failed\n");
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
nike_test_normeq_randomized(int bitsize, int iterations)
{
    int res = 0;
    ibz_t p, x, y, a, two_e, sum, prod;
    ibz_init(&p);
    ibz_init(&x);
    ibz_init(&y);
    ibz_init(&a);
    ibz_init(&two_e);
    ibz_init(&sum);
    ibz_init(&prod);
    for (int i = 0; i < iterations; i++) {
        ibz_generate_random_prime(&p, 1, bitsize, 30);
        ibz_pow(&two_e, &ibz_const_two, 2 * bitsize - 5);
        res = res | !nike_normeq(&x, &y, &a, &p, &two_e);
        ibz_mul(&sum, &x, &x);
        ibz_mul(&prod, &a, &a);
        ibz_add(&sum, &sum, &prod);
        ibz_mul(&prod, &y, &y);
        ibz_mul(&prod, &prod, &p);
        ibz_add(&sum, &sum, &prod);
        res = res | !(ibz_cmp(&sum, &two_e) == 0);
    }
    if (res) {
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
nike_test_sol2ec_randomized(int bitsize, int iterations)
{
    // return 0 if test succeeds
    int res = 1;
    if (!res) {
        printf("NIKE tool test sol2eq_randomized failed\n");
    }
    return (res);
}

int
main(int argc, char *argv[])
{
    uint32_t seed[12] = { 0 };
    int iterations = 10;
    int bitsize = 500;
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
    }

    if (help || iterations <= 0) {
        printf("Usage: %s [--iterations=<iterations>] [--bitsize=<bitsize>] [--seed=<seed>]\n", argv[0]);
        printf("Where <iterations> is the number of iterations used for testing; if not "
               "present, uses the default: %d)\n",
               iterations);
        printf("Where <bitsize> is the size used for testing; if not "
               "present, uses the default: %d)\n",
               bitsize);
        printf("Where <seed> is the random seed to be used; if not present, a random seed is "
               "generated\n");
        return 1;
    }

    if (!seed_set) {
        randombytes_select((unsigned char *)seed, sizeof(seed));
    }

    printf("--------------------------------------------------------------------------------\n\n");
    printf("Testing NIKE tools:\n\n");
    print_seed(seed);

    res = nike_test_normeq_static();
    res = res | nike_test_normeq_randomized(bitsize, iterations);
    // res = res | nike_test_sol2ec_randomized(bitsize, iterations);

    return res;
}
