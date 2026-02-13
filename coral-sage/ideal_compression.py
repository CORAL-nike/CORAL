from sage.all import Matrix, ZZ, isqrt, GF, legendre_symbol
from libs.pegasis.ideals import (
    element_multiply,
    element_norm,
    element_scale,
    short_vectors,
    short_vectors_plus,
)
import time

def are_equivalent(ideal1, ideal2):
    return ideal1.quadratic_form().reduced_form() == ideal2.quadratic_form().reduced_form()


def compress_ideal(ideal, ideal_norm, norm_bound, p, order, spr=None):
    """
    For a given ideal returns the smallest equivalent ideal of prime norm in a
    compressed form.

    Input:
    - I: an ideal as list of generators, where the element [a1, a2] is a1 + i*a2
    - N: the norm of I
    - B: bound on the (reduced) norm of the elements returned
    - p: the field prime
    - order: the order the ideal lives in
    - spr: isqrt(p) if precomputed # TODO: probably remove
    - cf_b: number of combinations to try in ShortVectors
    Output:
    - I: an ideal equivalent to I of prime norm, given as a list of lists
    - n: the norm of I
    """
    gens = [list(gen) for gen in ideal.gens()]

    if not spr:
        spr = isqrt(p)

    L = Matrix(ZZ, 2, [gens[0][0], spr * gens[0][1], gens[1][0], spr * gens[1][1]])

    L = L.LLL()
    L = [L[0], L[1]]
    cf_b = p.bit_length()
    for _, sh in short_vectors_plus(L, ideal_norm * norm_bound, cf_b=cf_b):
        cshel = [sh[0], -sh[1] / spr]
        if sh[1] % spr != 0:
            print("Non divisible")
        idl = [
            element_scale(element_multiply(gens[0], cshel, p), ideal_norm),
            element_scale(element_multiply(gens[1], cshel, p), ideal_norm),
            cshel,
        ]
        n = ZZ(element_norm(cshel, p) / ideal_norm)
        if n.is_prime():
            if legendre_symbol(- p, n) == 1:
                lam1 = GF(n)(- p).sqrt()
                w = order.gens()[1]
                idl = order * n + order * (ZZ(lam1) + w)
                if are_equivalent(ideal, idl):
                    return (n, 0)
                idl = order * n + order * (ZZ(-lam1) + w)
                assert are_equivalent(ideal, idl)
                return (n, 1)

def decompress_ideal(compressed_ideal, p, order):
    """
    Decompress an ideal given in compressed form.

    Input:
    - I: an ideal in compressed form as returned by compress_ideal
    - p: the field prime
    - order: the order the ideal lives in
    Output:
    - I: the decompressed ideal
    """
    n, sign = compressed_ideal
    w = order.gens()[1]
    if sign == 0:
        lam1 = GF(n)(- p).sqrt()
        idl = order * n + order * (ZZ(lam1) + w)
        return idl
    else:
        lam1 = GF(n)(- p).sqrt()
        idl = order * n + order * (ZZ(-lam1) + w)
        return idl

if __name__ == "__main__":
    from libs.pegasis.pegasis import PEGASIS
    import sys
    from tqdm import tqdm

    p_level = 2000
    reps = 10
    if len(sys.argv) > 1:
        reps = int(sys.argv[1])
        if len(sys.argv) > 2:
            p_level = int(sys.argv[2])
    print("Use sage compression.py [reps] [p_level]")

    print(f"Testing compression/decompression for p level {p_level} with {reps} repetitions")

    EGA = PEGASIS(p_level)  # 1000 | 1500 | 2000 | 4000
    p = EGA.p
    order = EGA.order
    print(f"The original is {ZZ(p).nbits()} bits")
    time_compression = 0
    time_decompression = 0
    compress_lens = []
    for _ in tqdm(range(reps)):
        ideal = EGA.sample_ideal()

        start = time.time()
        compressed_ideal = compress_ideal(ideal, ideal.norm(), isqrt(p) * p^2 , p, order)
        end = time.time()
        time_compression += end - start

        compress_len = compressed_ideal[0].nbits() + 1  # +1 for the sign bit
        # print(f"Compressed ideal size: {compress_len} bits")
        compress_lens.append(compress_len)

        start = time.time()
        ideal_decompressed = decompress_ideal(compressed_ideal, p, order)
        end = time.time()
        time_decompression += end - start
        assert are_equivalent(ideal, ideal_decompressed)
    print("Results over", reps, "repetitions:")
    print(f"Average compression time: {time_compression / reps:.4f} seconds")
    print(f"Average decompression time: {time_decompression / reps:.4f} seconds")
    print(f"Average compressed size: {sum(compress_lens) / reps:.2f} bits")
    print(f"Compression ratio: { ((sum(compress_lens) / reps)) / ZZ(p).nbits():.2f}")
    print("Max compressed size: ", max(compress_lens), "bits")


