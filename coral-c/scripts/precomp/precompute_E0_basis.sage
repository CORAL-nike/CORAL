#!/usr/bin/env sage
proof.all(False)  # faster

################################################################

from parameters import p, f

if p % 4 != 3:
    raise NotImplementedError('requires p ≡ 3 (mod 4)')

assert (1 << f).divides(p + 1)
Fp2.<i> = GF((p,2), modulus=[1,0,1])
Fp = GF(p)


#copy from coral-sage/utilities/volcano.py
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


E0 = EllipticCurve(Fp2, [1, 0])
E1 = EllipticCurve(Fp, [1, 0])
E1 = go_to_surface(E1)
A = E1.a_invariants()[1]
A24 =  (A+2)/4

from torsion_basis import even_torsion_basis_E0
P, Q = even_torsion_basis_E0(E0, f)

################################################################

from cformat import FpEl, Object, ObjectFormatter


E0 = EllipticCurve(Fp2, [1,0])

def Fp2_to_list(el):
    return [FpEl(int(c), p, True) for c in Fp2(el)]

objs = ObjectFormatter([
        Object('fp2_t', 'BASIS_E0_PX', Fp2_to_list(P.x())),
        Object('fp2_t', 'BASIS_E0_QX', Fp2_to_list(Q.x())),
        Object('curve_mg_fp_t', 'CURVE_E0',
                    [FpEl(int(A),p,True), FpEl(1,p,True),                    # ec_curve_t A, C
                     [FpEl(int(A24),p,True), FpEl(1,p,True)], "true"]       # ec_curve_t A24, is_A24_computed_and_normalized
                     )
    ])

################################################################

with open('include/e0_basis.h','w') as hfile:
    with open('e0_basis.c','w') as cfile:
        print(f'#include <fp2.h>', file=hfile)
        print(f'#include <e0_basis.h>', file=cfile)
        print(f'#include <ec.h>', file=hfile)

        objs.header(file=hfile)
        objs.implementation(file=cfile)
