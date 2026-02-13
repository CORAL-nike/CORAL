from random import choice

from sage.all import EllipticCurve, classical_modular_polynomial


def on_surface(E):
    try:
        E.torsion_basis(2)
    except ValueError:
        return False
    try:
        E.torsion_basis(4)
    except ValueError:
        return True

    return False


def random_elkies(j, ell):
    return choice(list(set(classical_modular_polynomial(ell, j=j).roots(j.parent(), multiplicities=False)) - set([j])))


def go_to_surface(E):
    if not on_surface(E):
        E = EllipticCurve(j=random_elkies(E.j_invariant(), 2)).montgomery_model()

    assert on_surface(E)
    return E


def go_to_floor(E):
    if on_surface(E):
        E = EllipticCurve(j=random_elkies(E.j_invariant(), 2)).montgomery_model()

    assert not on_surface(E)
    return E
