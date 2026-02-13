#ifndef EC_PARAMS_H
#define EC_PARAMS_H

#include <fp.h>

#define TORSION_EVEN_POWER 4084

// p+1 divided by the power of 2
extern const digit_t p_cofactor_for_2f[1];
extern const uint32_t FP_nqr[64];
#define P_COFACTOR_FOR_2F_BITLENGTH 6

#endif
