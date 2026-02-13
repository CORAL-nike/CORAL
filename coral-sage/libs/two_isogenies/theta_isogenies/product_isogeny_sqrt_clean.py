from ..utilities.strategy import optimised_strategy
from .gluing_isogeny import GluingThetaIsogeny
from .isogeny import ThetaIsogeny
from .isogeny_sqrt import ThetaIsogeny2, ThetaIsogeny4
from .isomorphism import SplittingIsomorphism
from .product_isogeny import EllipticProductIsogeny


class EllipticProductIsogenySqrt(EllipticProductIsogeny):
    """
    Compute the (2^n, 2^n)-isogeny between two elliptic products
    E1 x E2 -> E1' x E2'

    Does not require the input kernel to have order 2^(n+2) and
    instead the final two steps use (slower) isogenies which
    compute the necessary data using sqrts
    """

    def __init__(self, kernel, n, strategy=None, zeta=None, coerce=False):
        super().__init__(kernel, n, strategy=strategy, zeta=zeta, coerce=coerce)

    def get_strategy(self):
        return optimised_strategy(self.n - 2)

    def isogeny_chain(self, kernel):
        """ """
        # Extract CouplePoints from kernel
        Tp1, Tp2 = kernel

        # Store chain of (2,2)-isogenies
        isogeny_chain = []

        # Bookkeeping for optimal strategy
        strat_idx = 0
        level = [0]
        ker = (Tp1, Tp2)
        kernel_elements = [ker]

        for k in range(self.n - 2):
            prev = sum(level)
            ker = kernel_elements[-1]

            if k == 1 and self.coerce:
                ker[0].coerce_to(self.Fp)
                ker[1].coerce_to(self.Fp)

            while prev != (self.n - 3 - k):
                level.append(self.strategy[strat_idx])

                # Perform the doublings
                Tp1 = ker[0].double_iter(self.strategy[strat_idx])
                Tp2 = ker[1].double_iter(self.strategy[strat_idx])

                ker = (Tp1, Tp2)

                # Update kernel elements and bookkeeping variables
                kernel_elements.append(ker)
                prev += self.strategy[strat_idx]
                strat_idx += 1

            if k > 0:
                assert all([c.parent() == self.Fp for c in Tp1.coords() + Tp2.coords()])

            # Compute the codomain from the 8-torsion
            Tp1, Tp2 = ker

            if k == 0:
                phi = GluingThetaIsogeny(Tp1, Tp2)
            else:
                if __debug__ and not all([c in self.Fp for c in Tp1.coords() + Tp2.coords() + Th.coords()]):
                    print(f"In step {k}, the theta points *are not* over Fp")

                phi = ThetaIsogeny(Th, Tp1, Tp2)

            Th = phi.codomain()

            # Update the chain of isogenies
            isogeny_chain.append(phi)

            # Remove elements from list
            if k != self.n - 3:
                kernel_elements.pop()
            level.pop()

            # Push through points for the next step
            kernel_elements = [(phi(T1), phi(T2)) for T1, T2 in kernel_elements]

        # last 2 isogenies
        Tp1, Tp2 = kernel_elements[0]

        Tp1.coerce_to(self.Fp2)
        Tp2.coerce_to(self.Fp2)
        Th.coerce_to(self.Fp2)

        phi = ThetaIsogeny4(Th, Tp1, Tp2, hadamard=(False, False))
        isogeny_chain.append(phi)
        Th = phi.codomain()

        phi = ThetaIsogeny2(Th, hadamard=(True, False))
        isogeny_chain.append(phi)
        Th = phi.codomain()

        splitting_iso = SplittingIsomorphism(Th)
        isogeny_chain.append(splitting_iso)

        return isogeny_chain
