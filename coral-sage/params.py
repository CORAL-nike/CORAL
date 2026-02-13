"""
This module contains useful parameters for the Coral implementation.
"""

from sage.all import *

# Montgomery curve parameter so that the curve y^2 = x^3 + A*x^2 + x
# is at the surface, for p = 33 * 2**503 - 1
A500 = 846923981206860774667188923127927404866138431504857441785556155002892474407686465068998563030997400041497236660113832719116298437902864009587446028542241


class_number_500 = 30115877202646647376923175490783656066559648055235996072894708571193049262827
fact_class_number_500 = [101, 409, 3943, 17851, 5503889543116441, 1881879447649265588797081861911138024133873569131]


# "human name": (f, c) s.t. p = c * 2**f - 1
# These are the PEGASIS
params = {
    500: (503, 33, 391, None, 500),
    505: (500, 27, 390, None, 497),
    1000: (1004, 15, 642, 771, 1001),
    2000: (2026, 51, 1155, 1283, 2023),
    4000: (4084, 63, 2185, 2313, 4081),
}


e_params = {
        32: 30,
        128: 250,
        256: 450,
        500: 394,
        505: 392,
        1000: 642,
        2000: 1155,
        4000: 2185
}

p_pegasis = 2**503 * 33 - 1
from math import comb

def expected_counts_exact_k(n: int, t: int, k: int, rounding = True):
    """
    Returns a== E[# of classes seen exactly k times]
    when drawing t times with replacement in n class (uniformly).
    
    Formula:
        E[C_k] = n * C(t, k) * (1/n)^k * (1 - 1/n)^(t - k)
    """
    if not (0 <= k <= t):
        return None
    if n <= 0 or t < 0:
        raise ValueError("n must be >= 1 and t must be >= 0.")

    p = 1. / n
    one_minus_p = 1. - p
    out = n * comb(t, k) * (p ** k) * (one_minus_p ** (t - k))
    if rounding:
        return round(out, 1)
    return out

def prob_of_zero_coll(n: int, t: int, rounding = True):
    out = exp(- t*(t-1) / (2. * n))
    if not rounding:
        return out
    return round(out, 2)


def Ipots(p, e, class_number = None):
    """
    return the size of the Ipots sets quotiented on the class group
    """
    # first get the lb on the Ipots size
    ipots = 2**(e-2) / (sqrt(p) * (e-1) * log(2))
    t = round(ipots)
    if class_number is None:
        class_number = isqrt(p)
    size = class_number * ( 1 - exp(- t / class_number) )
    return round(size).bit_length(), t 

def min_entropy(t, n):
    # first check if no collisions
    pcoll = prob_of_zero_coll(n, t, rounding=False)
    if log(pcoll, 2) < -128 or pcoll==1:
        return round(t).bit_length()
    for k in range(1,100):
        out = expected_counts_exact_k(n, t, k, rounding = False)
        out = round(log(out,2.))
        if out < -128:
            # print('level k = ', k)
            if out is NaN:
                print(f"WARNING: out is NaN for k = {k}, t = {t}, n = {n}")
            print(f"WARNING: min_entropy found a k = {k} such that E[C_k] < 2^-128")
            return round(t/k).bit_length()
    print(f"WARNING: min_entropy did not find a k such that E[C_k] < 2^-128")
    print(out, pcoll)

def create_e(f, c, target_log_size, class_number = None):
    """
    return the smallest e such that Ipots(p, e) >= target_log_size
    """
    p = c * 2**f - 1
    e = f // 2 + target_log_size
    if e > f - 3:
        print(f"WARNING: for f = {f}, e = {target_log_size} is too big, bounding to f - 3")
        return  f - 3, None
    class_number = isqrt(p) if class_number is None else class_number
    s, t = Ipots(p, e)
    # while s < target_log_size:
    while s < target_log_size or min_entropy(t, class_number) < 128:
        e += 1
        if e > f - 3:
            print(f"WARNING: for f = {f}, e = {target_log_size} is too big, bounding to f - 3")
            return  f - 3, t
        s, t = Ipots(p, e)
    return e, t

if __name__ == "__main__":
    for f, c in params.values():

        if f < 500:
            continue
        p = c * 2**f - 1
        e128, t128 = create_e(f, c, 128)
        e256, t = create_e(f, c, 256)
        print(f"(f={f}, c={c}) => e128 = {e128}, e256 = {e256}")
