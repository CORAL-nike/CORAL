#include <stdio.h>
#include <stdlib.h>

#include "fp.h"
#include "encoded_sizes.h"

uint32_t
fp_parse(fp_t *parsed, char *hexstring, size_t len_hexstring)
{
    size_t n_bytes = (len_hexstring + 1) / 2;

    unsigned char bytes[FP_ENCODED_BYTES];
    memset(bytes, 0, FP_ENCODED_BYTES);

    // Must reverse: Bytes array is lsB to msB, but we write numbers msd to lsd
    // @Potential footgun: Assumes endianness is handled by `fp_decode`

    if (len_hexstring % 2) {
        if(sscanf(hexstring, "%1hhx", bytes + n_bytes - 1) != 1) {
            printf(":: fp_parser [ERROR] Could not parse \"%s\"\n", hexstring);
            return 0;
        }
    }

    int read;
    for (size_t i = (len_hexstring % 2); i < n_bytes; i++) {
        // Must check `sscanf` reads exactly 2 ("%2hhx" is "at least 1, at most 2")
        if(sscanf(hexstring + 2 * i - (len_hexstring % 2), "%2hhx%n", bytes + n_bytes - (i + 1), &read) != 1 || read != 2) {
            printf(":: fp_parser [ERROR] Could not parse \"%s\"\n", hexstring);
            return 0;
        }
    }

    fp_decode(parsed, bytes);
    return 1;
}

uint32_t
fp_fparse(fp_t *parsed, size_t *n_parsed, size_t parse, FILE *file)
{
    int c, pos = 0;
    *n_parsed = 0;
    char hexstring[2 * FP_ENCODED_BYTES];

    while ((*n_parsed < parse || parse == 0) && ((c = fgetc(file)) != EOF)) {
        if (pos > 2 * FP_ENCODED_BYTES) {
            printf(":: fp_parser [ERROR] Item %zu not delimited by space or newline: %s\n", *n_parsed + 1, hexstring);
            return 0;
        }

        if (c == ' ' || c == '\n') {
            if(!fp_parse(parsed + *n_parsed, hexstring, pos))
                return 0;
            (*n_parsed)++;
            pos = 0;
            if (c == '\n' || (parse != 0 && *n_parsed == parse))
                return 1;
        } else {
            hexstring[pos] = c;

            if (pos == 1 && (c == 'x' || c == 'X')) {
                if (hexstring[0] != '0') {
                    printf(":: fp_parser [ERROR] Bad prefix: %s...\n", hexstring);
                    return 0;
                }
                // i.e. prefix was "0x"
                pos = pos - 2;
            }

            pos++;
        }
    }
    printf(":: fp_parser [ERROR] Could not find final newline\n");
    return 0;
}
