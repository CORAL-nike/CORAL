from ..theta_structures.split_structure import SplitThetaStructure
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

    def __init__(self, kernel, n, strategy=None, zeta=None, split=None):
        # step at which we expect a split
        self.split_step = split
        if split == 1:
            print("Warning: first split after one step, may be inefficient!")
        super().__init__(kernel, n, strategy=strategy, zeta=zeta)

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
            print(f"Executing round {k + 1} out of {self.n} ")
            prev = sum(level)
            ker = kernel_elements[-1]

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

            # Compute the codomain from the 8-torsion
            Tp1, Tp2 = ker
            if k == 0 or k == 1 and self.split_step == 1:
                phi = GluingThetaIsogeny(Tp1, Tp2)
            else:
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
            # if self.split_step == 1 and k == 0:
            #     splitting_iso = SplittingIsomorphism(Th)
            #     isogeny_chain.append(splitting_iso)
            #     kernel_elements = [(splitting_iso(T1), splitting_iso(T2)) for T1, T2 in kernel_elements]
            #     Tp1 = splitting_iso(phi(Tp1))
            #     Tp2 = splitting_iso(phi(Tp2))
            #     Th = splitting_iso.codomain()
            #     # splitting_iso = SplitThetaStructure(splitting_iso.codomain())

            if self.split_step == k + 1:
                splitting_iso = SplittingIsomorphism(Th)
                splitting_iso = SplitThetaStructure(splitting_iso.codomain())
                jj = [EE.j_invariant() for EE in splitting_iso.curves()]
                print("j-invariants before final isogeny:")
                print(jj[0])
                print(jj[1])
                # breakpoint()

        # last 2 isogenies
        print(f"Executing round {self.n - 1} (final two isogenies)")
        Tp1, Tp2 = kernel_elements[0]
        phi = ThetaIsogeny4(Th, Tp1, Tp2, hadamard=(False, False))
        isogeny_chain.append(phi)
        Th = phi.codomain()
        if self.split_step == self.n - 1:
            splitting_iso = SplittingIsomorphism(Th)
            splitting_iso = SplitThetaStructure(splitting_iso.codomain())
            jj = [EE.j_invariant() for EE in splitting_iso.curves()]
            print("j-invariants before final isogeny:")
            print(jj[0])
            print(jj[1])
            breakpoint()
        print(f"Executing round {self.n} (final two isogenies)")
        phi = ThetaIsogeny2(Th, hadamard=(True, False))
        isogeny_chain.append(phi)
        Th = phi.codomain()

        splitting_iso = SplittingIsomorphism(Th)
        isogeny_chain.append(splitting_iso)

        return isogeny_chain


# class EllipticProductIsogenySqrtSplitOne(EllipticProductIsogenySqrt):
#     """
#     Compute the (2^n, 2^n)-isogeny between two elliptic products
#     E1 x E2 -> E1' x E2'
#
#     With the first split happening after one step
#     """
#
#     def __init__(self, kernel, n, strategy=None, zeta=None):
#         split = 1
#         super().__init__(kernel, n, strategy=strategy, zeta=zeta, split=None)
#
#
