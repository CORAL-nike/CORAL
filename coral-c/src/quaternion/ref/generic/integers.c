#include <quaternion.h>
#include "internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>

// adapt Miller-Rabin to compute sqrt of -1 for essentially cost f 1 MR iteration
// fails if unlucky: for ex if n!=2mod3
int
ibz_sqrt_minus_one(ibz_t *sqrt, const ibz_t *n, const uint32_t *primes, int primes_length)
{
    assert(mpz_sgn(*n) > 0);
    int res = 0;
    ibz_t gen, cmp;
    ibz_init(&gen);
    ibz_init(&cmp);
    int sn = ibz_get(n);
    if (sn % 4 != 1) {
        goto end;
    }
    if (sn % 8 == 5) {
        ibz_set(&gen, 2);
    } else if (sn % 3 == 2) {
        ibz_set(&gen, 3);
        ibz_neg(&gen, &gen);
    }
    // arbitrary generator choice
    else {
        int resloop = 0;
        if ((primes != NULL) && (primes_length > 0)) {
            for (int i = 0; i < primes_length; i++) {
                ibz_set(&gen, primes[i]);
                if (ibz_legendre(n, &gen) != 1) {
                    resloop = 1;
                    break;
                }
            }
        }
        if (!resloop) {
            goto end;
        }
    }
    mpz_sub_ui(cmp, *n, 1);
    int exp = ibz_two_adic(&cmp);
    ibz_div_2exp(sqrt, &cmp, exp);
    ibz_pow_mod(sqrt, &gen, sqrt, n);
    for (int i = 0; i < exp; i++) {
        ibz_pow_mod(&gen, sqrt, &ibz_const_two, n);
        if (ibz_cmp(&gen, &cmp) == 0) {
            res = 1;
            break;
        } else {
            mpz_swap(gen, *sqrt);
        }
    }
end:;
    ibz_finalize(&gen);
    ibz_finalize(&cmp);

    return (res);
}

// solves x^2 + n y^2 == p for positive integers x, y
// assumes that p is prime and -n mod p is a square
int
ibz_cornacchia_untested_prime(ibz_t *x, ibz_t *y, const ibz_t *p, const uint32_t *primes, int primes_length)
{
    ibz_t r0, r1, r2, a, prod;
    ibz_init(&r0);
    ibz_init(&r1);
    ibz_init(&r2);
    ibz_init(&a);
    ibz_init(&prod);

    int res = 0;

    // manage case p = 2 separately
    if (!ibz_cmp(p, &ibz_const_two)) {
        ibz_set(x, 1);
        ibz_set(y, 1);
        res = 1;
        goto done;
    }
    // manage case p = n separately
    if (ibz_is_one(p)) {
        ibz_set(x, 0);
        ibz_set(y, 1);
        res = 1;
        goto done;
    }

    // get sqrt of -n mod p
    // ibz_neg(&r2, ibz_const_one);
    if (!ibz_sqrt_minus_one(&r2, p, primes, primes_length))
        goto done;

    // run loop
    ibz_copy(&prod, p);
    ibz_copy(&r1, p);
    ibz_copy(&r0, p);
    while (ibz_cmp(&prod, p) >= 0) {
        ibz_div(&a, &r0, &r2, &r1);
        ibz_mul(&prod, &r0, &r0);
        ibz_copy(&r2, &r1);
        ibz_copy(&r1, &r0);
    }
    // test if result is solution
    ibz_sub(&a, p, &prod);
    if (!ibz_sqrt(y, &a))
        goto done;

    ibz_copy(x, &r0);
    ibz_mul(&a, y, y);
    ibz_add(&prod, &prod, &a);
    res = (ibz_cmp(&prod, p) == 0);

done:
    ibz_finalize(&r0);
    ibz_finalize(&r1);
    ibz_finalize(&r2);
    ibz_finalize(&a);
    ibz_finalize(&prod);
    return res;
}

// Random prime generation for tests
int
ibz_generate_random_prime(ibz_t *p, int is3mod4, int bitsize, int probability_test_iterations)
{
    assert(bitsize != 0);
    int found = 0;
    ibz_t two_pow, two_powp;

    ibz_init(&two_pow);
    ibz_init(&two_powp);
    ibz_pow(&two_pow, &ibz_const_two, (bitsize - 1) - (0 != is3mod4));
    ibz_pow(&two_powp, &ibz_const_two, bitsize - (0 != is3mod4));

    int cnt = 0;
    while (!found) {
        cnt++;
        if (cnt % 100000 == 0) {
            printf("Random prime generation is still running after %d attempts, this is not "
                   "normal! The expected number of attempts is %d \n",
                   cnt,
                   bitsize);
        }
        ibz_rand_interval(p, &two_pow, &two_powp);
        ibz_add(p, p, p);
        if (is3mod4) {
            ibz_add(p, p, p);
            ibz_add(p, &ibz_const_two, p);
        }
        ibz_add(p, &ibz_const_one, p);

        found = ibz_probab_prime(p, probability_test_iterations);
    }
    ibz_finalize(&two_pow);
    ibz_finalize(&two_powp);
    return found;
}

// solves x^2 + n y^2 == p for positive integers x, y
// assumes that p is prime and -n mod p is a square
int
ibz_cornacchia_prime(ibz_t *x, ibz_t *y, const ibz_t *n, const ibz_t *p)
{
    ibz_t r0, r1, r2, a, prod;
    ibz_init(&r0);
    ibz_init(&r1);
    ibz_init(&r2);
    ibz_init(&a);
    ibz_init(&prod);

    int res = 0;

    // manage case p = 2 separately
    if (!ibz_cmp(p, &ibz_const_two)) {
        if (ibz_is_one(n)) {
            ibz_set(x, 1);
            ibz_set(y, 1);
            res = 1;
        }
        goto done;
    }
    // manage case p = n separately
    if (!ibz_cmp(p, n)) {
        ibz_set(x, 0);
        ibz_set(y, 1);
        res = 1;
        goto done;
    }

    // test coprimality (should always be ok in our cases)
    ibz_gcd(&r2, p, n);
    if (!ibz_is_one(&r2))
        goto done;

    // get sqrt of -n mod p
    ibz_neg(&r2, n);
    if (!ibz_sqrt_mod_p(&r2, &r2, p))
        goto done;

    // run loop
    ibz_copy(&prod, p);
    ibz_copy(&r1, p);
    ibz_copy(&r0, p);
    while (ibz_cmp(&prod, p) >= 0) {
        ibz_div(&a, &r0, &r2, &r1);
        ibz_mul(&prod, &r0, &r0);
        ibz_copy(&r2, &r1);
        ibz_copy(&r1, &r0);
    }
    // test if result is solution
    ibz_sub(&a, p, &prod);
    ibz_div(&a, &r2, &a, n);
    if (!ibz_is_zero(&r2))
        goto done;
    if (!ibz_sqrt(y, &a))
        goto done;

    ibz_copy(x, &r0);
    ibz_mul(&a, y, y);
    ibz_mul(&a, &a, n);
    ibz_add(&prod, &prod, &a);
    res = (ibz_cmp(&prod, p) == 0);

done:
    ibz_finalize(&r0);
    ibz_finalize(&r1);
    ibz_finalize(&r2);
    ibz_finalize(&a);
    ibz_finalize(&prod);
    return res;
}

// returns product of a and b in Z[sqrt(-q)]
void
ibz_complex_mul(ibz_t *re_res,
                ibz_t *im_res,
                const ibz_t *re_a,
                const ibz_t *im_a,
                const ibz_t *re_b,
                const ibz_t *im_b,
                const ibz_t *q)
{
    ibz_t prod, re, im;
    ibz_init(&prod);
    ibz_init(&re);
    ibz_init(&im);
    ibz_mul(&re, re_a, re_b);
    ibz_mul(&prod, im_a, im_b);
    ibz_mul(&prod, &prod, q);
    ibz_sub(&re, &re, &prod);
    ibz_mul(&im, re_a, im_b);
    ibz_mul(&prod, im_a, re_b);
    ibz_add(&im, &im, &prod);
    ibz_copy(im_res, &im);
    ibz_copy(re_res, &re);
    ibz_finalize(&prod);
    ibz_finalize(&re);
    ibz_finalize(&im);
}

// multiplies res by a^e with res and a in Z[sqrt(-q)]
void
ibz_complex_mul_by_complex_power(ibz_t *re_res,
                                 ibz_t *im_res,
                                 const ibz_t *re_a,
                                 const ibz_t *im_a,
                                 const ibz_t *q,
                                 int64_t exp)
{
    ibz_t re_x, im_x;
    ibz_init(&re_x);
    ibz_init(&im_x);
    ibz_set(&re_x, 1);
    ibz_set(&im_x, 0);
    for (int i = 0; i < 64; i++) {
        ibz_complex_mul(&re_x, &im_x, &re_x, &im_x, &re_x, &im_x, q);
        if ((exp >> (63 - i)) & 1) {
            ibz_complex_mul(&re_x, &im_x, &re_x, &im_x, re_a, im_a, q);
        }
    }
    ibz_complex_mul(re_res, im_res, re_res, im_res, &re_x, &im_x, q);
    ibz_finalize(&re_x);
    ibz_finalize(&im_x);
}

// multiplies to res the result of the solutions of cornacchia for prime depending on valuation val
// (prime-adic valuation)
int
ibz_cornacchia_extended_prime_loop(ibz_t *re_res, ibz_t *im_res, const ibz_t *q, int64_t prime, int64_t val)
{
    ibz_t re, im, p;
    ibz_init(&re);
    ibz_init(&im);
    ibz_init(&p);
    ibz_set(&p, prime);
    int res = ibz_cornacchia_prime(&re, &im, q, &p);
    if (res) {
        ibz_complex_mul_by_complex_power(re_res, im_res, &re, &im, q, val);
    }
    ibz_finalize(&re);
    ibz_finalize(&im);
    ibz_finalize(&p);
    return (res);
}

int
ibz_cornacchia_extended(ibz_t *x, ibz_t *y, const ibz_t *n, const ibz_cornacchia_extended_params_t *params)
{
    ibz_t nodd, g, r, p, qz;
    ibz_init(&nodd);
    ibz_init(&r);
    ibz_init(&g);
    ibz_init(&p);
    ibz_init(&qz);
    uint64_t *valuations = calloc(params->prime_list_length, sizeof(uint64_t));

    int res = 0;
    ibz_set(&qz, params->q);
    // if a prime p divides n and -q is not a square mod p, extended Cornacchia can't solve the
    // equation
    if (params->bad_primes_prod != NULL) {
        ibz_gcd(&g, n, params->bad_primes_prod);
        if (!ibz_is_one(&g))
            goto done;
    }

    // get the valuations and the unfactored part by attempting division by all given primes
    ibz_copy(&nodd, n);
    for (unsigned i = 0; i < params->prime_list_length; ++i) {
        ibz_set(&r, 0);
        ibz_set(&p, params->prime_list[i]);
        ibz_copy(&g, &nodd);
        while (ibz_is_zero(&r)) {
            ++valuations[i];
            ibz_copy(&nodd, &g);
            ibz_div(&g, &r, &nodd, &p);
        }
        --valuations[i];
    }

    // we hope the 'unfactored' part is a prime p and -q is a square mod p
    if (ibz_is_one(&nodd)) {
        // simple case: the 'unfactored' part is 1, solution is obvious
        ibz_set(x, 1);
        ibz_set(y, 0);
        res = 1;
    } else {
        // we hope the 'unfactored' part is a prime p and -q is a square mod p so that we can use
        // Cornacchia
        if (!ibz_probab_prime(&nodd, params->primality_test_iterations))
            goto done;
        ibz_neg(&qz, &qz);
        if (1 != ibz_legendre(&qz, &nodd))
            goto done;
        ibz_neg(&qz, &qz);

        res = ibz_cornacchia_prime(x, y, &qz, &nodd);
        if (!res)
            goto done;
    }

    // note: this loop only runs until the first failure (res == 0)
    for (unsigned i = 0; res && i < params->prime_list_length; i++) {
        if (0 == valuations[i])
            continue;
        res = ibz_cornacchia_extended_prime_loop(x, y, &qz, params->prime_list[i], valuations[i]);
    }
#ifndef NDEBUG
    if (res) {
        ibz_t test, sum;
        ibz_init(&test);
        ibz_init(&sum);
        ibz_mul(&sum, x, x);
        ibz_mul(&test, y, y);
        ibz_mul(&test, &qz, &test);
        ibz_add(&sum, &test, &sum);
        assert(ibz_cmp(&sum, n) == 0);
        ibz_finalize(&test);
        ibz_finalize(&sum);
    }
#endif

done:
    free(valuations);
    ibz_finalize(&nodd);
    ibz_finalize(&r);
    ibz_finalize(&g);
    ibz_finalize(&p);
    ibz_finalize(&qz);
    return res;
}

// requires positive input
int
ibz_cornacchia_trial_division(ibz_t *quotient,
                              ibz_cornacchia_trial_division_output_t *factorization,
                              const ibz_t *product,
                              const ibz_cornacchia_trial_division_consts_t *consts)
{
    ibz_copy(quotient, product);
    int rem = 0;
    int res = 1;
    ibz_t q;
    ibz_init(&q);

    // try out void mpz_divexact_ui (mpz_t q, const mpz_t n, unsigned long d)
    // and  int mpz_divisible_ui_p (const mpz_t n, unsigned long int d)
    for (int i = 0; i < consts->bad_primes_length; i++) {
        // unsigned long int mpz_fdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)
        rem = 0;
        factorization->bad_half_exponents[i] = 0;
        while (rem == 0) {
            rem = mpz_fdiv_q_ui(q, (*quotient), consts->bad_primes[i]);
            if (rem == 0) {
                ibz_copy(quotient, &q);
                factorization->bad_half_exponents[i] = factorization->bad_half_exponents[i] + 1;
            }
        }
        if (!(factorization->bad_half_exponents[i] & 1)) {
            factorization->bad_half_exponents[i] = factorization->bad_half_exponents[i] >> 1;
        } else {
            res = 0;
            goto fin;
        }
    }

    for (int i = 0; i < consts->good_primes_length; i++) {
        // unsigned long int mpz_fdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)
        rem = 0;
        factorization->good_half_exponents[i] = 0;
        factorization->good_remainders[i] = 0;
        while (rem == 0) {
            rem = mpz_fdiv_q_ui(q, (*quotient), consts->good_primes[i]);
            if (rem == 0) {
                ibz_copy(quotient, &q);
                factorization->good_half_exponents[i] = factorization->good_half_exponents[i] + 1;
            }
        }
        factorization->good_remainders[i] = (char)(factorization->good_half_exponents[i] & 1);
        factorization->good_half_exponents[i] = factorization->good_half_exponents[i] >> 1;
    }
    if (ibz_get(quotient) % 4 != 1) {
        res = 0;
        goto fin;
    }
    // if ((!ibz_is_one(quotient)) && (!ibz_probab_prime(quotient, consts->primality_test_iterations))) {
    //     res = 0;
    //     goto fin;
    // }

fin:;
    ibz_finalize(&q);
    return (res);
}

// returns product of a and b in Z[i]
void
ibz_complex_mul_i(ibz_t *re_res,
                  ibz_t *im_res,
                  const ibz_t *re_a,
                  const ibz_t *im_a,
                  const ibz_t *re_b,
                  const ibz_t *im_b)
{
    ibz_t prod, re, im;
    ibz_init(&prod);
    ibz_init(&re);
    ibz_init(&im);
    ibz_mul(&re, re_a, re_b);
    ibz_mul(&prod, im_a, im_b);
    ibz_sub(&re, &re, &prod);
    ibz_mul(&im, re_a, im_b);
    ibz_mul(&prod, im_a, re_b);
    ibz_add(&im, &im, &prod);
    ibz_copy(im_res, &im);
    ibz_copy(re_res, &re);
    ibz_finalize(&prod);
    ibz_finalize(&re);
    ibz_finalize(&im);
}

// only supports odd numbers so far
int
ibz_cornacchia_extended_trials(ibz_t *x,
                               ibz_t *y,
                               ibz_cornacchia_trial_division_output_t *factorization,
                               const ibz_t *n,
                               const ibz_cornacchia_trial_division_consts_t *consts)
{
    ibz_t q, prod, pow, p, xp, yp;
    ibz_init(&p);
    ibz_init(&q);
    ibz_init(&xp);
    ibz_init(&yp);
    ibz_init(&prod);
    ibz_init(&pow);
    ibz_set(&prod, 1);
    int res = 1;
    int cornacchia_bound = 64;
    if (consts->good_primes_length < cornacchia_bound)
        cornacchia_bound = consts->good_primes_length;

    // even numbers are not yet supported
    // uint32_t exp2 = ibz_two_adic(n);
    // ibz_div_2exp(&q,n,exp2);
    if ((ibz_cmp(n, &ibz_const_zero) < 0) || ((ibz_get(n) & 1) == 0)) {
        res = 0;
        goto end;
    }

    int trial = ibz_cornacchia_trial_division(&q, factorization, n, consts);
    if (!trial) {
        res = 0;
        goto end;
    }
    if (ibz_is_one(&q)) {
        ibz_set(x, 1);
        ibz_set(y, 0);
    } else {
        res = ibz_cornacchia_untested_prime(x, y, &q, consts->good_primes, 64);
        // res = ibz_cornacchia_prime(x, y, &ibz_const_one, &q);
    }
    if (!res)
        goto end;

    ibz_set(&prod, 1);
    for (int i = 0; i < consts->bad_primes_length; i++) {
        if (factorization->bad_half_exponents[i] != 0) {
            ibz_set(&p, consts->bad_primes[i]);
            ibz_pow(&pow, &p, factorization->bad_half_exponents[i]);
            ibz_mul(&prod, &prod, &pow);
        }
    }
    for (int i = 0; i < consts->good_primes_length; i++) {
        if (factorization->good_half_exponents[i] != 0) {
            ibz_set(&p, consts->good_primes[i]);
            ibz_pow(&pow, &p, factorization->good_half_exponents[i]);
            ibz_mul(&prod, &prod, &pow);
        }
        if (factorization->good_remainders[i] != 0) {
            ibz_set(&p, consts->good_primes[i]);
            ibz_cornacchia_prime(&xp, &yp, &ibz_const_one, &p);
            ibz_complex_mul_i(x, y, x, y, &xp, &yp);
        }
    }
    ibz_mul(x, x, &prod);
    ibz_mul(y, y, &prod);

end:;
    ibz_finalize(&q);
    ibz_finalize(&p);
    ibz_finalize(&xp);
    ibz_finalize(&yp);
    ibz_finalize(&prod);
    ibz_finalize(&pow);
    return (res);
}