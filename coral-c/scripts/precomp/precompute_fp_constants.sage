from parameters import p,lvl
from sage.all import log, ceil

def get_limbs(p, lvl ,arith=None):
    if(arith == "broadwell"):
        # lvl1
        if p == 0x4ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff:
            return 4
        # lvl3
        elif p == 0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff:
            return 6
        # lvl5
        elif p == 0x1afffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff:
            return 8
        else:
            raise ValueError(f'Invalid broadwell prime \"{p}\"')

    if arith == "ref" or arith is None:
        # lvl1
        if p == 0x4ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff:
            return 5
        # lvl3
        elif p == 0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff:
            return 7
        # lvl5
        elif p == 0x1afffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff:
            return 9
        else:
            fprime = open("../../../../src/gf/ref/"+lvl+"/fp_"+lvl + "_"+str(64)+".c","r")
            r = fprime.readlines()
            fprime.close()
            for l in r:
                if("#define Nlimbs " in l):
                    rd = l.split(" ")[2]
                    if(rd[-1]=="\n"):
                        rd = rd[:-1]
                    return(int(rd.strip(),10))
        raise ValueError(f'Invalid prime \"{p}\"')


NWORDS_FIELD =get_limbs(p,lvl)
BITS = ceil(log(p,2**64))*64
NWORDS_ORDER =BITS//64
if(p==0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff):
    BITS=384
LOG2P =ceil(log(log(p,2),2))


defs = dict()

defs['BITS'] = BITS
defs['LOG2P'] = LOG2P
defs['NWORDS_ORDER'] = NWORDS_ORDER

################################################################

with open('include/fp_constants.h','w') as hfile:
    if p in [0x4ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff, 0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff,0x1afffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff]:
        NWORDS_FIELD_BROADWELL = get_limbs(p,lvl,arith="broadwell")
        print(f'#if defined(SQISIGN_GF_IMPL_BROADWELL)', file=hfile)
        print(f'#define NWORDS_FIELD {NWORDS_FIELD_BROADWELL}', file=hfile)
        print(f'#else', file=hfile)
        print(f'#define NWORDS_FIELD {NWORDS_FIELD}', file=hfile)
        print(f'#endif', file=hfile)
    else: 
        print('#define NWORDS_FIELD {NWORDS_FIELD}'.format(NWORDS_FIELD = NWORDS_FIELD), file=hfile)
    for k,v in defs.items():
        v = ZZ(v)
        print(f'#define {k} {v}', file=hfile)


