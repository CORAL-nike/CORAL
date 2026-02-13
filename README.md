# CORAL


This repository contains a sage and a C implementation of Coral, as well as code to replicate our various experiments. 

## C implementation

In folder `coral-c`, it is based on the code from [qlapoti](https://github.com/KULeuven-COSIC/Qlapoti), itself based on the NIST round 2 submission of SQIsign. This code was heavily adapted, in particular most quaternion subroutines were removed and isogenies over Fp added. Like SQIsign, it requires the GMP library.

For the tests, compile as advised in the SQIsign_README.md, with a ref build. Then go into the build/src/nike/<prime>/test` folder, and run experiments. Some additional tests not mentionned in our draft are just empty files (in particular some related to the full NIKE are not yet implemented). The primes starting with p_<bitsize> are the pegasis primes, the ones called lvl<x> the SQIsign-NIST2 primes.


## Sage implementation and experiments

The folder `coral-sage` contains a Sagemath implementation of Coral. 
[Sagemath 10.7](https://www.sagemath.org/) has been used for the experiments.

The file `coral.py` is the main reference. To run it you need to have Sagemath
installed, and then you can run it with `sage -python coral.py`. It will run one
ideal sampling and action evaluation for the lowest $512$-bit security level and
print statistics.
Other important files are:

- `params.py` contains the parameters used in the paper, as well as some code to
  compute them.
- `ideal2d.py` contains the code to sample ideal class group elements.

This repository contains imported modules in `libs/` from the following repositories:

- [pegasis](https://github.com/pegasis4d/pegasis), needed only for compatibility tests.
- [two-isogenies](https://github.com/ThetaIsogenies/two-isogenies/tree/main/Theta-SageMath).

Some changes have been made to accomodate the doubling-reuse, the Fp-coercion
techniques mentioned in the paper and importing the modules.

### Testing

The folder contains also a few tests mentioned in the paper:

- `experimental_analysis.ipynb` shows how to compute the experimental analysis
  shown in the paper.
- `sampling_distribution.py` can be used to compute the data used for the
  analysis. A bash script `collison.sh` is also provided. The data is stored in
  the `data` folder.
- `test_order.py` tests the order of the ideal class group elements sampled by
  `ideal2d.py` methods and checks that they are generator of the class group.
- `test_compatibility.py` tests the compatibility of the implementation with the
  implementation of [PEGASIS](https://github.com/pegasis4d/pegasis).

### Benchmarks

Compile the C code with

```
cmake -S coral-c -B coral-c/build -DSQISIGN_BUILD_TYPE=ref -DCMAKE_BUILD_TYPE=release
make -j -C coral-c/build
```

Then to perform benchmarks, pass the benchmark vectors on stdin

```
cat coral-c/src/precomp/ref/p_500/nike_action_benchmark_vectors.txt | ./coral-c/build/src/nike/ref/p_500/test/test_nike_action_p_500
cat coral-c/src/precomp/ref/p_1000/nike_action_benchmark_vectors.txt | ./coral-c/build/src/nike/ref/p_1000/test/test_nike_action_p_1000
cat coral-c/src/precomp/ref/p_2000/nike_action_benchmark_vectors.txt | ./coral-c/build/src/nike/ref/p_2000/test/test_nike_action_p_2000
cat coral-c/src/precomp/ref/p_4000/nike_action_benchmark_vectors.txt | ./coral-c/build/src/nike/ref/p_4000/test/test_nike_action_p_4000
```

To also benchmark the broadwell architecture edit the `CMakeLists.txt` from

```
SET(SVARIANT_S "p_test;p_500;p_1000;p_2000;p_4000;lvl1;lvl3;lvl5")
# SET(SVARIANT_S "lvl5")
```

to

```
# SET(SVARIANT_S "p_test;p_500;p_1000;p_2000;p_4000;lvl1;lvl3;lvl5")
SET(SVARIANT_S "lvl5")
```

And re-compile the code, this time specifying broadwell

```
cmake -S coral-c -B coral-c/build -DSQISIGN_BUILD_TYPE=broadwell -DCMAKE_BUILD_TYPE=release
```

Finally

```
cat coral-c/src/precomp/ref/lvl5/nike_action_benchmark_vectors.txt | ./coral-c/build/src/nike/ref/lvl5/test/test_nike_action_lvl5
```

New benchmark vectors can be produced using `coral-sage/c_test_vectors.py`, e.g.

```
cd coral-sage
python3 c_test_vectors.py -p500
```
