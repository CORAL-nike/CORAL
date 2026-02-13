#!/usr/bin/env python 3

from contextlib import suppress
from dataclasses import dataclass
from random import Random
from time import perf_counter_ns

from sage.all import inverse_mod, is_prime, isqrt, valuation

from libs.pegasis.coin import sum_of_2_squares


# Mimicing a C-struct (with a bit of syntactic sugar)
@dataclass
class Ideal2d:
    q: int
    x: int
    y: int
    e: int
    p: int
    postinit: int = True

    # Derived values
    def __post_init__(self):
        if not self.postinit:
            return

        if (2 ** (self.e - 1) - self.q) ** 2 + self.x**2 + self.p * self.y**2 != 2 ** (2 * (self.e - 1)):
            raise AssertionError("Invalid TwoDimIdeal")

        self.v = int(valuation((self.q + self.x) ** 2 + self.p * self.y**2, 2) - 2)

        # Later will compute
        #   (q + x + y) * P1
        #   (q - x - y) * P2
        # Since we only care about this couple up to scalars,
        # and P1.order() == P2.order() == 2*(e + 2) we actually compute
        #   inverse_mod((q + x - y), 2**(e + 2)) * (q + x + y) * P1
        #   gcd(q + x - y, 2**(e + 2)) * P2

        s = self.q - self.x - self.y
        self.s_v2 = valuation(s, 2)
        s_even, s_odd = 2**self.s_v2, s // 2**self.s_v2
        self.P1_mult = inverse_mod(s_odd, 2 ** (self.e + 2)) * (self.q + self.x + self.y)
        self.P2_mult = s_even
        self.P1_mult = self.P1_mult % 2 ** (self.e + 2)
        self.P2_mult = self.P2_mult % 2 ** (self.e + 2)

        t = self.q - self.x + self.y
        self.t_v2 = valuation(t, 2)
        t_even, t_odd = 2**self.t_v2, t // 2**self.t_v2
        self.Q1_mult = inverse_mod(t_odd, 2 ** (self.e + 2)) * (self.q + self.x - self.y)
        self.Q2_mult = t_even

        self.Q1_mult = self.Q1_mult % 2 ** (self.e + 2)
        self.Q2_mult = self.Q2_mult % 2 ** (self.e + 2)

    # Make unpackable (https://stackoverflow.com/a/37837754)
    def __iter__(self):
        return iter((self.q, self.x, self.y, self.e))

    def to_sage_ideal(self, order, w):
        return order * self.q + order * (self.x + w * self.y)

    def conjugate(self):
        # Recall (q, \gamma) and (2^e - q, \overline{\gamma}) generate (as algebras) equivalent ideals
        return Ideal2d(q=2**self.e - self.q, x=self.x, y=self.y, e=self.e, p=self.p)


def sample_ideal2d(p, e, stats=None, try_y=None, bit_y=0, rng_seed=None, only_prime_M=False, postinit=True):
    """Solve Coral norm equation

    i.e. find x, y, a s.t. a^2 + x^2 + p y^2 = 2^{2(e-1)}

    Return TwoDimIdeal representation
    """

    if try_y is not None or stats is not None:
        y_samples = 0

    if stats is not None:
        stats.init()

    rng = Random(rng_seed)

    N = 2 ** (2 * (e - 1))
    bound = 2 ** (e - 1) // isqrt(p)

    if bound < 1:
        raise ValueError("Bound too small")

    y_time = 0
    M_time = 0
    prime_time = 0
    cornacchia_time = 0

    a = None
    while a is None and (try_y is None or y_samples < 1):
        if try_y is not None or stats is not None:
            y_samples += 1

        if stats is not None:
            t0 = perf_counter_ns()

        if try_y is not None:
            y = try_y
            if y > bound:
                raise AssertionError(f"y too big: {y = }, {bound = }")
        else:
            y = 2 * rng.randint(0, bound // 2) + 1

        if stats is not None:
            t1 = perf_counter_ns()
            y_time += t1 - t0

        # M % 4 = 1
        M = N - p * y**2

        if stats is not None:
            t2 = perf_counter_ns()
            M_time += t2 - t1

        # Lazy eval prevents expensive primality test
        M_is_prime = only_prime_M and is_prime(M)

        if stats is not None:
            t3 = perf_counter_ns()
            prime_time += t3 - t2

        # Must come after time has been added
        if only_prime_M and not M_is_prime:
            continue

        with suppress(ValueError):
            # Will discard `M`s that are too composite
            # a, x = Cornacchia(M, 1)

            worked, a, x = sum_of_2_squares(M, early_abort=True)
            if not worked:
                a = None

        if stats is not None:
            t4 = perf_counter_ns()
            cornacchia_time += t4 - t3

        if only_prime_M and not is_prime(M):
            continue

    if stats is not None:
        stats.update("Norm Equation Samples", nondiff=y_samples)
        stats.update("y time", value=[y_time / 10**6])
        stats.update("M time", value=[M_time / 10**6])
        stats.update("Primality testing", value=[prime_time / 10**6])
        stats.update("Cornacchia", value=[cornacchia_time / 10**6])

    # Can only happen if `try_y` is passed
    if a is None:
        return None

    # One of a, x must be odd, because `M` is odd
    if a % 2 == 0:
        a, x = x, a

    assert a % 2 == 1
    assert a**2 + x**2 + p * y**2 == N

    # Matching notation of paper
    q = 2 ** (e - 1) - a

    # Note on randomising the output
    # We will generate the ideals
    #   (q, x + yw)
    #   (2^e - q, x + yw) (= (2^{e-1}(a - 1), x + yw))
    # of norms q and 2^e - q
    # The latter turns out to be the inverse of the first in the class group
    # When `w` has trace 0 (e.g. Frobenius), `x + yw` conjugates to `x - yw`
    # (This is no longer the case when `w = (\pi + 1)/2`!)
    #
    # Hence, we randomise the sign choice on x (or y, but not both)
    if try_y is None:
        y = rng.choice([-1, 1]) * y
    elif bit_y == 1:
        y = -y

    secret_key = Ideal2d(q=q, x=x, y=y, e=e, p=p, postinit=postinit)

    if stats is not None:
        return secret_key, stats

    return secret_key
