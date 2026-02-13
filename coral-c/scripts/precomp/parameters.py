#!/usr/bin/env python3
from sage.all import *
proof.all(False)  # faster

import re
for l in open('sqisign_parameters.txt'):
    if(l[:3]=="lvl"):
        v = (l.split("=")[1].strip())
        if(v[-1]=="\n"):
            v = v[:-1].strip()
        globals()["lvl"] = v
    else:
        for k in ('p', 'num_orders','primes_per_order', 'good_primes','bad_primes'):
            m = re.search(rf'^\s*{k}\s*=\s*([x0-9a-f]+)', l)
            if m:
                v = ZZ(m.groups()[0], 0)
                globals()[k] = v

f = (p+1).valuation(2)

__all__ = ['lvl','p', 'f', 'num_orders', 'primes_per_order','good_primes','bad_primes']

