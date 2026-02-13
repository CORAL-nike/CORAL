#!/usr/bin/env bash

# test that sequentially computes all the possible y-samples
for p in 32 45 64; do
    for e in 14 15 16 18 19; do
      echo "p = $p e = $e"
      sage sampling_distribution.py -p "$p" -e "$e"
    done
done

# test used to check uniform distribution of the samples.
# they just perform random sampling.
for p in 32 45 64; do
    for e in 14 15 16 18 19; do
      echo "p = $p e = $e"
      sage sampling_distribution.py -p "$p" -e "$e" -r
    done
done
