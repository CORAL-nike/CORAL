#include <hd_splitting_transforms.h>

#define FP2_ZERO 0
#define FP2_ONE 1
#define FP2_I 2
#define FP2_MINUS_ONE 3
#define FP2_MINUS_I 4

const int EVEN_INDEX[10][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 2}, {2, 0}, {2, 1}, {3, 0}, {3, 3}};
const int CHI_EVAL[4][4] = {{1, 1, 1, 1}, {1, -1, 1, -1}, {1, 1, -1, -1}, {1, -1, -1, 1}};
const fp2_t FP2_CONSTANTS[5] = {{
#if 0
#elif RADIX == 64
{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
#endif
, 
#if 0
#elif RADIX == 64
{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
#endif
}, {
#if 0
#elif RADIX == 64
{0x1111, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x100000000000}
#endif
, 
#if 0
#elif RADIX == 64
{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
#endif
}, {
#if 0
#elif RADIX == 64
{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
#endif
, 
#if 0
#elif RADIX == 64
{0x1111, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x100000000000}
#endif
}, {
#if 0
#elif RADIX == 64
{0xfffffffffffeeee, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xdfffffffffff}
#endif
, 
#if 0
#elif RADIX == 64
{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
#endif
}, {
#if 0
#elif RADIX == 64
{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
#endif
, 
#if 0
#elif RADIX == 64
{0xfffffffffffeeee, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xfffffffffffffff, 0xdfffffffffff}
#endif
}};
const precomp_basis_change_matrix_t SPLITTING_TRANSFORMS[10] = {{{{FP2_ONE, FP2_I, FP2_ONE, FP2_I}, {FP2_ONE, FP2_MINUS_I, FP2_MINUS_ONE, FP2_I}, {FP2_ONE, FP2_I, FP2_MINUS_ONE, FP2_MINUS_I}, {FP2_MINUS_ONE, FP2_I, FP2_MINUS_ONE, FP2_I}}}, {{{FP2_ONE, FP2_ZERO, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ZERO, FP2_ZERO, FP2_ONE}, {FP2_ZERO, FP2_ZERO, FP2_ONE, FP2_ZERO}, {FP2_ZERO, FP2_MINUS_ONE, FP2_ZERO, FP2_ZERO}}}, {{{FP2_ONE, FP2_ZERO, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ONE, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ZERO, FP2_ZERO, FP2_ONE}, {FP2_ZERO, FP2_ZERO, FP2_MINUS_ONE, FP2_ZERO}}}, {{{FP2_ONE, FP2_ZERO, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ONE, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ZERO, FP2_ONE, FP2_ZERO}, {FP2_ZERO, FP2_ZERO, FP2_ZERO, FP2_MINUS_ONE}}}, {{{FP2_ONE, FP2_ONE, FP2_ONE, FP2_ONE}, {FP2_ONE, FP2_MINUS_ONE, FP2_MINUS_ONE, FP2_ONE}, {FP2_ONE, FP2_ONE, FP2_MINUS_ONE, FP2_MINUS_ONE}, {FP2_MINUS_ONE, FP2_ONE, FP2_MINUS_ONE, FP2_ONE}}}, {{{FP2_ONE, FP2_ZERO, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ONE, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ZERO, FP2_ZERO, FP2_ONE}, {FP2_ZERO, FP2_ZERO, FP2_ONE, FP2_ZERO}}}, {{{FP2_ONE, FP2_ONE, FP2_ONE, FP2_ONE}, {FP2_ONE, FP2_MINUS_ONE, FP2_ONE, FP2_MINUS_ONE}, {FP2_ONE, FP2_MINUS_ONE, FP2_MINUS_ONE, FP2_ONE}, {FP2_MINUS_ONE, FP2_MINUS_ONE, FP2_ONE, FP2_ONE}}}, {{{FP2_ONE, FP2_ONE, FP2_ONE, FP2_ONE}, {FP2_ONE, FP2_MINUS_ONE, FP2_ONE, FP2_MINUS_ONE}, {FP2_ONE, FP2_MINUS_ONE, FP2_MINUS_ONE, FP2_ONE}, {FP2_ONE, FP2_ONE, FP2_MINUS_ONE, FP2_MINUS_ONE}}}, {{{FP2_ONE, FP2_ONE, FP2_ONE, FP2_ONE}, {FP2_ONE, FP2_MINUS_ONE, FP2_ONE, FP2_MINUS_ONE}, {FP2_ONE, FP2_ONE, FP2_MINUS_ONE, FP2_MINUS_ONE}, {FP2_MINUS_ONE, FP2_ONE, FP2_ONE, FP2_MINUS_ONE}}}, {{{FP2_ONE, FP2_ZERO, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ONE, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ZERO, FP2_ONE, FP2_ZERO}, {FP2_ZERO, FP2_ZERO, FP2_ZERO, FP2_ONE}}}};
const precomp_basis_change_matrix_t NORMALIZATION_TRANSFORMS[6] = {{{{FP2_ONE, FP2_ZERO, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ONE, FP2_ZERO, FP2_ZERO}, {FP2_ZERO, FP2_ZERO, FP2_ONE, FP2_ZERO}, {FP2_ZERO, FP2_ZERO, FP2_ZERO, FP2_ONE}}}, {{{FP2_ZERO, FP2_ZERO, FP2_ZERO, FP2_ONE}, {FP2_ZERO, FP2_ZERO, FP2_ONE, FP2_ZERO}, {FP2_ZERO, FP2_ONE, FP2_ZERO, FP2_ZERO}, {FP2_ONE, FP2_ZERO, FP2_ZERO, FP2_ZERO}}}, {{{FP2_ONE, FP2_ONE, FP2_ONE, FP2_ONE}, {FP2_ONE, FP2_MINUS_ONE, FP2_ONE, FP2_MINUS_ONE}, {FP2_ONE, FP2_ONE, FP2_MINUS_ONE, FP2_MINUS_ONE}, {FP2_ONE, FP2_MINUS_ONE, FP2_MINUS_ONE, FP2_ONE}}}, {{{FP2_ONE, FP2_MINUS_ONE, FP2_MINUS_ONE, FP2_ONE}, {FP2_MINUS_ONE, FP2_MINUS_ONE, FP2_ONE, FP2_ONE}, {FP2_MINUS_ONE, FP2_ONE, FP2_MINUS_ONE, FP2_ONE}, {FP2_ONE, FP2_ONE, FP2_ONE, FP2_ONE}}}, {{{FP2_MINUS_ONE, FP2_I, FP2_I, FP2_ONE}, {FP2_I, FP2_MINUS_ONE, FP2_ONE, FP2_I}, {FP2_I, FP2_ONE, FP2_MINUS_ONE, FP2_I}, {FP2_ONE, FP2_I, FP2_I, FP2_MINUS_ONE}}}, {{{FP2_ONE, FP2_I, FP2_I, FP2_MINUS_ONE}, {FP2_I, FP2_ONE, FP2_MINUS_ONE, FP2_I}, {FP2_I, FP2_MINUS_ONE, FP2_ONE, FP2_I}, {FP2_MINUS_ONE, FP2_I, FP2_I, FP2_ONE}}}};
