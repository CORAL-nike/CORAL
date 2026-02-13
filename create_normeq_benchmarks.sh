#!/bin/bash

set -e

cmake -S coral-c -B coral-c/__cmake__ -DSQISIGN_BUILD_TYPE=ref -DCMAKE_BUILD_TYPE=release
make -j -C coral-c/__cmake__

hyperfine './__cmake__/src/nike/ref/p_500/test/bench_normeq_p_500 --bitsize=392 --iterations=25 --test=0'
hyperfine './__cmake__/src/nike/ref/p_1000/test/bench_normeq_p_1000 --bitsize=642 --iterations=25 --test=0'
hyperfine './__cmake__/src/nike/ref/p_2000/test/bench_normeq_p_2000 --bitsize=1155 --iterations=25 --test=0'
hyperfine './__cmake__/src/nike/ref/p_4000/test/bench_normeq_p_4000 --bitsize=2185 --iterations=25 --test=0'

cmake -S coral-c/flint -B coral-c/flint/__cmake__ -DCMAKE_BUILD_TYPE=release
make -j -C coral-c/flint/__cmake__

cd coral-c/flint/__cmake__
hyperfine './norm_equation --bitsize=500 --iterations=25 --good-primes=150 --bad-primes=200 --aggressive=1'
hyperfine './norm_equation --bitsize=1000 --iterations=25 --good-primes=300 --bad-primes=400 --aggressive=1'
hyperfine './norm_equation --bitsize=2000 --iterations=25 --good-primes=1000 --bad-primes=2500 --aggressive=1'
hyperfine './norm_equation --bitsize=4000 --iterations=25 --good-primes=2500 --bad-primes=5000 --aggressive=1'
