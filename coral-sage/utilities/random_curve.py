#!/usr/bin/env python3

from random import choice

from sage.all import GF, EllipticCurve, Primes, classical_modular_polynomial, kronecker_symbol, proof

proof.all(False)


def splits(ell, prime):
    return kronecker_symbol(-prime, ell) == 1


def random_maximal_curve(p):
    Fp2 = GF((p, 2), names="i", modulus=[1, 0, 1])
    j0 = Fp2(1728)
    j1 = choice(list(set(classical_modular_polynomial(2, j0).roots(Fp2, multiplicities=False)) - set([j0])))

    for _ in range(p.bit_length() // 8):
        mp2 = classical_modular_polynomial(2, j1)
        x = mp2.parent().gens()[0]
        j0, j1 = j1, choice((mp2 // (x - j0)).roots(Fp2, multiplicities=False))

    E = EllipticCurve(j=j1)
    if E.cardinality() != (p + 1) ** 2:
        E = E.quadratic_twist()
        assert E.cardinality() == (p + 1) ** 2
        return E

    return E


def random_maximal_fp_curve(p):
    Fp = GF(p)
    primes = Primes()
    ells = []
    i = 1
    while len(ells) < 4:
        while not splits(primes[i], p):
            i += 1
        assert splits(primes[i], p)
        ells += [primes[i]]
        i += 1

    vector = [choice(range(16)) for _ in range(len(ells))]

    j0 = Fp(1728)

    for ell, exp in zip(ells, vector, strict=True):
        mp = classical_modular_polynomial(ell)
        for e in range(exp):
            if e == 0:
                j1 = choice(mp(Y=j0).univariate_polynomial().roots(Fp, multiplicities=False))
            else:
                poly = mp(Y=j1).univariate_polynomial()
                x = poly.parent().gens()[0]
                try:
                    j0, j1 = j1, choice((poly // (x - j0)).roots(Fp, multiplicities=False))
                except IndexError:
                    print(poly.factor(), j0)
                    exit(1)

    E = EllipticCurve(j=j1)

    if E.cardinality() != (p + 1):
        E = E.quadratic_twist()

    assert E.cardinality() == (p + 1)

    return E
