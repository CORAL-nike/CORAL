#!/usr/bin/env python3
import sys, itertools
from math import ceil, floor, log
import sage.all
from parameters import lvl

class Ibz:
    def __init__(self, v):
        self.v = int(v)
    def _literal(self, sz):
        val = int(self.v)
        sgn = val < 0
        num_limbs = (abs(val).bit_length() + sz-1) // sz if val else 0
        limbs = [(abs(val) >> sz*i) & (2**sz-1) for i in range(num_limbs or 1)]
        data = {
                '._mp_alloc': 0,
                '._mp_size': (-1)**sgn * num_limbs,
                '._mp_d': '(mp_limb_t[]) {' + ','.join(map(hex,limbs)) + '}',
            }
        return '{{' + ', '.join(f'{k} = {v}' for k,v in data.items()) + '}}'

class FpEl:
    ref_p5248_radix_map  = { 16: 13, 32: 29, 64: 51 }
    ref_p65376_radix_map = { 16: 13, 32: 28, 64: 55 }
    ref_p27500_radix_map = { 16: 13, 32: 29, 64: 57 }
    def __init__(self, n, p, montgomery=True):
        self.n = n
        self.p = p
        self.montgomery = montgomery
    def __get_radix(self, word_size, lvl ,arith=None):
        if arith == "ref" or arith is None:
            # lvl1
            if self.p == 0x4ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff:
                return self.ref_p5248_radix_map[word_size]
            # lvl3
            elif self.p == 0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff:
                return self.ref_p65376_radix_map[word_size]
            # lvl5
            elif self.p == 0x1afffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff:
                return self.ref_p27500_radix_map[word_size]
            else:
                fprime = open("../../../../src/gf/ref/"+lvl+"/fp_"+lvl + "_"+str(word_size)+".c","r")
                r = fprime.readlines()
                fprime.close()
                for l in r:
                    if("#define Radix " in l):
                        rd = l.split(" ")[2]
                        if(rd[-1]=="\n"):
                            rd = rd[:-1]
                        return(int(rd.strip(),10))
            raise ValueError(f'Invalid prime \"{self.p}\"')
        elif arith == "broadwell":
            return word_size
        raise ValueError(f'Invalid arithmetic implementation type \"{arith}\"')
    def __get_limbnum(self, word_size, lvl ,arith=None):
        if arith == "ref" or arith is None:
            # lvl1
            if self.p in [0x4ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff, 0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff,0x1afffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff]:
                return ceil(log(self.p, 2**self.__get_radix(word_size,lvl,arith=arith)))
            else:
                fprime = open("../../../../src/gf/ref/"+lvl+"/fp_"+lvl + "_"+str(word_size)+".c","r")
                r = fprime.readlines()
                fprime.close()
                for l in r:
                    if("#define Nlimbs " in l):
                        rd = l.split(" ")[2]
                        if(rd[-1]=="\n"):
                            rd = rd[:-1]
                        return(int(rd.strip(),10))
            raise ValueError(f'Invalid prime \"{self.p}\"')
        elif arith == "broadwell":
            if self.p in [0x4ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff, 0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff,0x1afffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff]:
                return ceil(log(self.p, 2**self.__get_radix(word_size,lvl,arith=arith)))
            else: 
                raise ValueError(f'Invalid prime for broadwell \"{self.p}\"')
        raise ValueError(f'Invalid arithmetic implementation type \"{arith}\"')
    def _literal(self, sz, lvl, arith=None):
        if(arith=="broadwell"):
            radix = self.__get_radix(sz,lvl, arith=arith)
            l = 1 + floor(log(self.p, 2**radix))
            if self.montgomery:
                    R = 2**(radix * ceil(log(self.p, 2**radix)))
            else:
                R = 1
            el = (self.n * R) % self.p
            vs = [(int(el) >> radix*i) % 2**radix for i in range(l)]
            return '{' + ', '.join(map(hex, vs)) + '}'
        radix = self.__get_radix(sz,lvl, arith=arith)
        limbnum = self.__get_limbnum(sz,lvl,arith=None)
        l = limbnum
        #l = 1 + floor((int(self.p)).bit_length()/ radix)
        #l = 1 + floor(log(self.p, 2**radix))

        # If we're using Montgomery representation, we need to multiply
        # by the Montgomery factor R = 2^nw (n = limb number, w = radix)
        if self.montgomery:
            R = 2**(radix * limbnum)
        else:
            R = 1
        el = (self.n * R) % self.p
        vs = [(int(el) >> radix*i) % 2**radix for i in range(l)]
        return '{' + ', '.join(map(hex, vs)) + '}'

class Object:
    def __init__(self, ty, name, obj):
        if '[' in ty:
            idx = ty.index('[')
            depth = ty.count('[]')
            def rec(os, d):
                assert d >= 0
                if not d:
                    return ()
                assert isinstance(os,list) or isinstance(os,tuple)
                r, = {rec(o, d-1) for o in os}
                return (len(os),) + r
            dims = rec(obj, depth)
            self.ty = ty[:idx], ''.join(f'[{d}]' for d in dims)
        else:
            self.ty = ty, ''
        self.name = name
        self.obj = obj

    def _declaration(self):
        return f'extern const {self.ty[0]} {self.name}{self.ty[1]};'

    def _literal(self):
        def rec(obj):
            sz = 64
            if isinstance(obj, int):
                if obj < 256: return str(obj)
                else: return hex(obj)
            if isinstance(obj, sage.all.Integer):
                if obj < 256: return str(obj)
                else: return hex(obj)
            if isinstance(obj, Ibz):
                literal = "\n#if 0"
                literal += f"\n#elif GMP_LIMB_BITS == {sz}"
                literal += f"\n{obj._literal(sz)}"
                return literal + "\n#endif\n"
            if isinstance(obj, FpEl):
                literal = "\n#if 0"
                literal += f"\n#elif RADIX == {sz}"
                if obj.p in [0x4ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff, 0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff,0x1afffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff]:
                    literal += "\n#if defined(SQISIGN_GF_IMPL_BROADWELL)"
                    literal += f"\n{obj._literal(sz, lvl,'broadwell')}"
                    literal += "\n#else"
                    literal += f"\n{obj._literal(sz, lvl,'ref')}"
                    literal += "\n#endif\n"
                else:
                    literal += f"\n{obj._literal(sz, lvl,'ref')}"
                return literal + "\n#endif\n"
            if isinstance(obj, list) or isinstance(obj, tuple):
                return '{' + ', '.join(map(rec, obj)) + '}'
            if isinstance(obj, str):
                return obj
            raise NotImplementedError(f'unknown type {type(obj)} in Formatter')
        return rec(self.obj)

    def _definition(self):
        return f'const {self.ty[0]} {self.name}{self.ty[1]} = ' + self._literal() + ';'

class ObjectFormatter:
    def __init__(self, objs):
        self.objs = objs

    def header(self, file=None):
        for obj in self.objs:
            assert isinstance(obj, Object)
            print(obj._declaration(), file=file)

    def implementation(self, file=None):
        for obj in self.objs:
            assert isinstance(obj, Object)
            print(obj._definition(), file=file)
