// SPDX-License-Identifier: Apache-2.0

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <rng.h>
#include <bench_test_arguments.h>
#ifdef TARGET_BIG_ENDIAN
#include <tutil.h>
#endif

#ifdef ENABLE_CT_TESTING
#include <valgrind/memcheck.h>
#endif

#ifdef ENABLE_CT_TESTING
static void print_hex(const unsigned char *hex, int len) {
    unsigned char *copy  = calloc(len, 1);
    memcpy(copy, hex, len); // make a copy that we can tell valgrind is okay to leak
    VALGRIND_MAKE_MEM_DEFINED(copy, len);

    for (int i = 0; i < len;  ++i) {
        printf("%02x", copy[i]);
    }
    printf("\n");
    free(copy);
}
#else
static void print_hex(const unsigned char *hex, int len) {
    for (int i = 0; i < len;  ++i) {
        printf("%02x", hex[i]);
    }
    printf("\n");
}
#endif

// test for the wrapped NIKE
static int test_NIKE(unsigned long long in_msglen) {
    int res = 0;
    return res;
}

int main(int argc, char *argv[]) {
    uint32_t seed[12] = { 0 };
    int help = 0;
    int seed_set = 0;
    int msglen_set = 0;
    int res = 0;
    unsigned long long msglen = 32;

    for (int i = 1; i < argc; i++) {
        unsigned int _msglen;

        if (!help && strcmp(argv[i], "--help") == 0) {
            help = 1;
            continue;
        }

        if (!seed_set && !parse_seed(argv[i], seed)) {
            seed_set = 1;
            continue;
        }

        if (!msglen_set && sscanf(argv[i], "--msglen=%u", &_msglen) == 1) {
            msglen = (unsigned long long) _msglen;
            msglen_set = 1;
        }
    }

    if (help) {
        printf("Usage: %s [--seed=<seed>]\n", argv[0]);
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

    res = test_NIKE(msglen);

    if (res != 0) {
        printf("test failed for %s\n", argv[1]);
    }
    return res;
}
