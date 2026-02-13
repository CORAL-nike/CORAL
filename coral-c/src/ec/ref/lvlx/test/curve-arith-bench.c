#include <bench.h>
#include <bench_test_arguments.h>
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

#include "test_extras.h"
#include <ec.h>
#include <rng.h>

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

uint64_t
bench__xpoint_eqw_fp2_dbl(unsigned int Nbench)
{
    uint64_t cycles0, cycles1;
    unsigned int i;
    xpoint_eqw_fp2_t P[Nbench], A24[Nbench];
    for (i = 0; i < Nbench; i++) {
        fp2_random_test(&(P[i].x));
        fp2_random_test(&(P[i].z));
        fp2_random_test(&(A24[i].x));
        fp2_random_test(&(A24[i].z));
    }
    cycles0 = cpucycles();
    for (i = 0; i < Nbench; i++) {
        _xpoint_eqw_fp2_dbl(&P[i], &P[i], &A24[i]);
    }
    cycles1 = cpucycles();
    return cycles1 - cycles0;
}


int
main(int argc, char *argv[])
{
    uint32_t seed[12] = { 0 };
    int iterations = 100 * SQISIGN_TEST_REPS;
    int help = 0;
    int seed_set = 0;

#ifndef NDEBUG
    fprintf(stderr,
            "\x1b[31mIt looks like SQIsign was compiled with assertions enabled.\n"
            "This will severely impact performance measurements.\x1b[0m\n");
#endif

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
    }

    if (help || iterations <= 0) {
        printf("Usage: %s [--iterations=<iterations>] [--seed=<seed>]\n", argv[0]);
        printf("Where <iterations> is the number of iterations used for benchmarking; if not "
               "present, uses the default: %d)\n",
               iterations);
        printf("Where <seed> is the random seed to be used; if not present, a random seed is "
               "generated\n");
        return 1;
    }

    if (!seed_set) {
        randombytes_select((unsigned char *)seed, sizeof(seed));
    }

    print_seed(seed);

#if defined(TARGET_BIG_ENDIAN)
    for (int i = 0; i < 12; i++) {
        seed[i] = BSWAP32(seed[i]);
    }
#endif

    randombytes_init((unsigned char *)seed, NULL, 256);
    cpucycles_init();

    printf("Benchmarking elliptic curve arithmetic for " STRINGIFY(SQISIGN_VARIANT) ":\n\n");

    uint64_t cycles;

    cycles = bench__xpoint_eqw_fp2_dbl(10 * iterations);
    printf("Bench _xpoint_eqw_fp2_dbl_A24:\t%" PRIu64 " cycles\n", cycles / (10 * iterations));

    return 0;
}
