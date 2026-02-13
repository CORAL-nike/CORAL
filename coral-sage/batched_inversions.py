#!/usr/bin/env python3


def batched_inversion_2(a, b, c, d):
    batch = 1 / (b * d)
    q, r = a * d * batch, c * b * batch
    assert q == a / b
    assert r == c / d
    return q, r


def batched_inversion_4(a, b, c, d, e, f, g, h):
    batch = 1 / (b * d * f * h)

    bd = b * d
    bf = b * f
    df = d * f

    dfh = df * h
    bfh = bf * h
    bdh = bd * h
    bdf = bd * f

    q = a * dfh * batch
    r = c * bfh * batch
    s = e * bdh * batch
    t = g * bdf * batch

    assert q == a / b
    assert r == c / d
    assert s == e / f
    assert t == g / h

    return q, r, s, t


def prod(it, exclude=None):
    p = 1
    excluded = False
    for i in it:
        if i == exclude and not excluded:
            excluded = True
            continue

        p *= i
    return p


def batched_inversion(pairs):
    inv = 1 / prod(d for _, d in pairs)
    quotients = []

    for n, d in pairs:
        quotients += [n * prod([_d for _, _d in pairs], exclude=d) * inv]

    return quotients


if __name__ == "__main__":
    from random import randint

    a, b, c, d = randint(1, 10), randint(1, 10), randint(1, 10), randint(1, 10)
    pairs = [[a, b], [c, d]]
    quotients = batched_inversion(pairs)
    assert all([round(q * d, 3) == round(n, 3) for q, (n, d) in zip(quotients, pairs, strict=True)])
