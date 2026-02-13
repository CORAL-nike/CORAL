#include "quaternion_tests.h"
#include <rng.h>

// int ibz_cornacchia_prime(ibz_t *x, ibz_t *y, const ibz_t *n, const ibz_t *p);
int
quat_test_randomized_ibz_cornacchia_prime(int bitsize, int n_bound, int iterations)
{
    int res = 0;
    ibz_t x, y, n, prod, c_res, p;
    int32_t rand_fact;
    int randret = 0;
    ibz_init(&x);
    ibz_init(&y);
    ibz_init(&n);
    ibz_init(&p);
    ibz_init(&prod);
    ibz_init(&c_res);
    for (int iter = 0; iter < iterations; iter++) {
        // Sample small n for cornacchia
        rand_fact = 0;
        while (rand_fact < 1) {
            randret = randret | randombytes((unsigned char *)&rand_fact, sizeof(int32_t));
            if (randret != 0)
                goto fin;
            if (rand_fact < 0)
                rand_fact = -rand_fact;
            rand_fact = rand_fact % n_bound;
            ibz_set(&n, rand_fact);
        }
        randret = randret | !ibz_generate_random_prime(&p, 0, bitsize, 32);
        if (randret != 0)
            goto fin;
        // If the legendre symbol is ok, Cornacchia should sometimes be able to solve
        ibz_neg(&prod, &n);
        ibz_mod(&prod, &prod, &p);
        if (ibz_legendre(&prod, &p) > -1) {
            //  If there is output, check the output is correct
            if (ibz_cornacchia_prime(&x, &y, &n, &p)) {
                ibz_mul(&c_res, &x, &x);
                ibz_mul(&prod, &y, &y);
                ibz_mul(&prod, &prod, &n);
                ibz_add(&c_res, &c_res, &prod);
                res = res || (0 != ibz_cmp(&p, &c_res));
            }
        } else {
            // Otherwise Cornacchia should fail
            res = res || (ibz_cornacchia_prime(&x, &y, &n, &p));
        }
    }
fin:;

    if (randret != 0) {
        printf("Randomness failed in quaternion unit test with randomization for "
               "ibz_cornacchia_prime\n");
    }
    if (res != 0) {
        printf("Quaternion unit test with randomization for ibz_cornacchia_prime failed\n");
    }
    ibz_finalize(&x);
    ibz_finalize(&y);
    ibz_finalize(&n);
    ibz_finalize(&p);
    ibz_finalize(&prod);
    ibz_finalize(&c_res);
    return res;
}

// run all previous tests
int
quat_test_with_randomization(void)
{
    int res = 0;
    printf("\nRunning randomized tests from quaternion module\n");
    res = res | quat_test_randomized_ibz_cornacchia_prime(128, 6, 10);
    return (res);
}
