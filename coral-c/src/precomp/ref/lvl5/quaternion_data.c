#include <stddef.h>
#include <stdint.h>
#include <quaternion_data.h>
const short QUAT_small_primes_where_neg_q_square[101] = {2, 5, 13, 17, 29, 37, 41, 53, 61, 73, 89, 97, 101, 109, 113, 137, 149, 157, 173, 181, 193, 197, 229, 233, 241, 0x101, 0x10d, 0x115, 0x119, 0x125, 0x139, 0x13d, 0x151, 0x15d, 0x161, 0x175, 0x185, 0x18d, 0x191, 0x199, 0x1a5, 0x1b1, 0x1c1, 0x1c9, 0x1cd, 0x1fd, 0x209, 0x21d, 0x22d, 0x239, 0x241, 0x251, 0x259, 0x265, 0x269, 0x281, 0x28d, 0x295, 0x2a1, 0x2a5, 0x2bd, 0x2c5, 0x2dd, 0x2f5, 0x2f9, 0x301, 0x305, 0x31d, 0x329, 0x335, 0x33d, 0x355, 0x359, 0x36d, 0x371, 0x3a1, 0x3a9, 0x3ad, 0x3b9, 0x3d1, 0x3e5, 0x3f1, 0x3f5, 0x3fd, 0x409, 0x419, 0x425, 0x42d, 0x445, 0x449, 0x455, 0x45d, 0x469, 0x481, 0x49d, 0x4a9, 0x4b1, 0x4bd, 0x4c1, 0x4cd, 0x4d5};
const uint32_t QUAT_good_primes_list[150] = {5, 13, 17, 29, 37, 41, 53, 61, 73, 89, 97, 101, 109, 113, 137, 149, 157, 173, 181, 193, 197, 229, 233, 241, 0x101, 0x10d, 0x115, 0x119, 0x125, 0x139, 0x13d, 0x151, 0x15d, 0x161, 0x175, 0x185, 0x18d, 0x191, 0x199, 0x1a5, 0x1b1, 0x1c1, 0x1c9, 0x1cd, 0x1fd, 0x209, 0x21d, 0x22d, 0x239, 0x241, 0x251, 0x259, 0x265, 0x269, 0x281, 0x28d, 0x295, 0x2a1, 0x2a5, 0x2bd, 0x2c5, 0x2dd, 0x2f5, 0x2f9, 0x301, 0x305, 0x31d, 0x329, 0x335, 0x33d, 0x355, 0x359, 0x36d, 0x371, 0x3a1, 0x3a9, 0x3ad, 0x3b9, 0x3d1, 0x3e5, 0x3f1, 0x3f5, 0x3fd, 0x409, 0x419, 0x425, 0x42d, 0x445, 0x449, 0x455, 0x45d, 0x469, 0x481, 0x49d, 0x4a9, 0x4b1, 0x4bd, 0x4c1, 0x4cd, 0x4d5, 0x4e1, 0x4fd, 0x509, 0x511, 0x515, 0x529, 0x551, 0x55d, 0x565, 0x581, 0x595, 0x599, 0x5ad, 0x5c9, 0x5d1, 0x5d5, 0x60d, 0x611, 0x63d, 0x641, 0x649, 0x64d, 0x655, 0x665, 0x679, 0x685, 0x69d, 0x6a1, 0x6ad, 0x6b9, 0x6c5, 0x6cd, 0x6d9, 0x6f1, 0x6fd, 0x709, 0x745, 0x751, 0x755, 0x761, 0x76d, 0x779, 0x78d, 0x79d, 0x7b5, 0x7c9, 0x7cd, 0x7e1, 0x7ed, 0x805};
const uint32_t QUAT_bad_primes_list[200] = {3, 7, 11, 19, 23, 31, 43, 47, 59, 67, 71, 79, 83, 103, 107, 127, 131, 139, 151, 163, 167, 179, 191, 199, 211, 223, 227, 239, 251, 0x107, 0x10f, 0x11b, 0x133, 0x137, 0x14b, 0x15b, 0x167, 0x16f, 0x17b, 0x17f, 0x1a3, 0x1af, 0x1b7, 0x1bb, 0x1cf, 0x1d3, 0x1df, 0x1e7, 0x1eb, 0x1f3, 0x1f7, 0x20b, 0x223, 0x233, 0x23b, 0x24b, 0x257, 0x25f, 0x26b, 0x277, 0x283, 0x287, 0x293, 0x2ab, 0x2b3, 0x2cf, 0x2d7, 0x2e3, 0x2e7, 0x2ef, 0x313, 0x32b, 0x337, 0x33b, 0x347, 0x35b, 0x35f, 0x373, 0x377, 0x38b, 0x38f, 0x397, 0x3b3, 0x3c7, 0x3cb, 0x3d7, 0x3df, 0x3fb, 0x407, 0x40f, 0x41b, 0x427, 0x43f, 0x443, 0x44f, 0x463, 0x47f, 0x48b, 0x493, 0x4a3, 0x4c7, 0x4cf, 0x4eb, 0x4ff, 0x503, 0x50b, 0x517, 0x51b, 0x527, 0x52f, 0x557, 0x577, 0x58f, 0x593, 0x59f, 0x5a7, 0x5ab, 0x5b3, 0x5bf, 0x5cb, 0x5cf, 0x5db, 0x5e7, 0x5f3, 0x5fb, 0x607, 0x617, 0x61f, 0x623, 0x62b, 0x62f, 0x647, 0x653, 0x65b, 0x67f, 0x683, 0x6a3, 0x6bb, 0x6d3, 0x6df, 0x6f7, 0x6fb, 0x713, 0x71f, 0x727, 0x737, 0x74b, 0x74f, 0x757, 0x773, 0x78b, 0x79f, 0x7bb, 0x7c3, 0x7cf, 0x7d3, 0x7db, 0x7eb, 0x7f7, 0x80f, 0x823, 0x827, 0x833, 0x83f, 0x853, 0x85f, 0x883, 0x89b, 0x89f, 0x8bf, 0x8c3, 0x8cb, 0x8db, 0x8ef, 0x907, 0x923, 0x92b, 0x92f, 0x943, 0x94f, 0x95f, 0x96b, 0x977, 0x98f, 0x99b, 0x9a3, 0x9c7, 0x9e3, 0x9eb, 0x9ef, 0x9f7, 0xa13, 0xa1f, 0xa57, 0xa63, 0xa67, 0xa6f, 0xa7b, 0xa7f, 0xa8b};
const ibz_t QUAT_prods_of_bad_primes[1] = {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 14, ._mp_d = (mp_limb_t[]) {0x4c6ceae8c3ebca85,0xad5fe3da133e40c5,0x8e433144c9df5d,0x3409ef47d4d6caaf,0x283205661003b9d1,0xf00588fa23eebc41,0x13571e3ac5b0f459,0x68d7978850e5ee1b,0xfc90ce7ca1e084e3,0x3826ece3073b91b2,0x142c2ada2fd1ca91,0xc99ac56813b8574d,0x7e44a077ae0037db,0x5f5c51e80}}}
#endif
};
const ibz_t QUAT_prime_cofactor = 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0x33,0x0,0x0,0x0,0x0,0x0,0x0,0x200000000000000}}}
#endif
;
const quat_alg_t QUATALG_PINFTY = {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 8, ._mp_d = (mp_limb_t[]) {0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0xffffffffffffffff,0x1afffffffffffff}}}
#endif
};
const quat_p_extremal_maximal_order_t EXTREMAL_ORDERS[1] = {{{
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}, 1}};
const quat_left_ideal_t CONNECTING_IDEALS[1] = {{{
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, {{
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x2}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
}}}, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, &MAXORD_O0}};
const quat_alg_elem_t CONJUGATING_ELEMENTS[1] = {{
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, {
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 1, ._mp_d = (mp_limb_t[]) {0x1}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
, 
#if 0
#elif GMP_LIMB_BITS == 64
{{._mp_alloc = 0, ._mp_size = 0, ._mp_d = (mp_limb_t[]) {0x0}}}
#endif
}}};
