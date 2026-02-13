#!/usr/bin/env python3

from argparse import ArgumentParser

from sage.all import proof
from time_arithmetic import time_arithmetic

proof.all(False)

parser = ArgumentParser()
parser.add_argument("-f", "--file", type=str, default=None)
parser.add_argument("-s", "--samples", type=int, default=8)
parser.add_argument("-p", type=int, default=500)
args = parser.parse_args()

if args.file is None:
    print("Must specify `--file`. Exiting")
    exit(1)

repeats = 2**args.samples
params = {500: (500, 3 * 5 * 7**2 * 11**2), 503: (503, 33), 1000: (1004, 15), 2000: (2026, 51), 4000: (4084, 63)}
f, c = params[args.p]
p = c * 2**f - 1

try:
    with open(args.file) as _file:
        file = _file.readlines()
except FileNotFoundError:
    print(f"Cannot read '{args.file}'. Exiting")
    exit(1)


counts = {
    "Fp_add": 0.0,
    "Fp_sub": 0.0,
    "Fp_neg": 0.0,
    "Fp_mul": 0.0,
    "Fp_div": 0.0,
    "Fp_inv": 0.0,
    "Fp_pow": 0.0,
    "Fp_squ": 0.0,
    "Fp2_add": 0.0,
    "Fp2_sub": 0.0,
    "Fp2_neg": 0.0,
    "Fp2_mul": 0.0,
    "Fp2_div": 0.0,
    "Fp2_inv": 0.0,
    "Fp2_pow": 0.0,
    "Fp2_squ": 0.0,
}

for line in file:
    line = line.split()
    if len(line) < 3:
        continue
    if line[1] in counts:
        counts[line[1]] = int(float(line[3]))
    if f"{line[0]} {line[1]} {line[2]}" == "Total action time":
        total_time = float(line[4])


times = time_arithmetic(p, 2**args.samples)

Fp_time = sum([counts[attr] * times[attr] for attr in times if attr[:3] == "Fp_"])
Fp2_time = sum([counts[attr] * times[attr] for attr in times if attr[:4] == "Fp2_"])

print(f"Fp2 is {Fp2_time / 10**9 / total_time * 100:.4f}% of total time")
print(f"Fp+Fp2 is {(Fp_time + Fp2_time) / 10**9 / (total_time) * 100:.4f}% of total time")
