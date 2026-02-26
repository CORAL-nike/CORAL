# CORAL


This repository contains a sage and a C implementation of Coral, as well as code to replicate our various experiments. 

## C implementation

In folder `coral-c`, it is based on the code from [qlapoti](https://github.com/KULeuven-COSIC/Qlapoti), itself based on the NIST round 2 submission of SQIsign. This code was heavily adapted, in particular most quaternion subroutines were removed and isogenies over Fp added. Like SQIsign, it requires the GMP library.

For the tests, compile as advised in the SQIsign_README.md, with a ref build. Then go into the build/src/nike/<prime>/test` folder, and run experiments. Some additional tests not mentionned in our draft are just empty files (in particular some related to the full NIKE are not yet implemented). The primes starting with p_<bitsize> are the pegasis primes, the ones called lvl<x> the SQIsign-NIST2 primes.

### Benchmarks

To see all benchmarks for all primes, call `python3 testbench.py`.

Broadwell benchmarks will be automatically attempted if the machine is detected
to be `x86`.

```
usage: testbench.py [-h] [-k] [-a] [-n] [-i ITERATIONS] [-s] [params ...]

CORAL Benchmarking/Test Script
--------------------------------------------------------------------------------

Without any options, this script computes 100 iterations of all benchmarks
for all primes.

Broadwell benchmarks will be automatically attempted if the machine is
detected to have an `x86` architecture (there may be false positives).

--------------------------------------------------------------------------------

positional arguments:
  params

options:
  -h, --help            show this help message and exit
  -k, --nike            Perform key-exchange benchmarks
  -a, --action          Perform action benchmarks
  -n, --normeq          Perform norm equation benchmarks
  -i, --iterations ITERATIONS
                        Number of iterations
  -s, --single          Show every single run (not just average)

--------------------------------------------------------------------------------

There are three routines which can be benchmarked and tested

1. The full key-exchange (option `--nike`)
2. A single group action (option `--action`)
  (Verifies PEGASIS compatibility by comparing to pre-computed test vectors)
3. A single norm equation solution (option `--normeq`)

Recall:
- "Keygen" = 1 norm equation solution + 1 group action computation
- "Secret Derivation" = 1 group action computation

To limit to a selection of primes, pass the desired primes chosen from the
list

  * ['500b', '500', '1000', '2000', '4000']

Primes suffixed with "b" are ones with assembly optimised finite field
arithmetic (requires Broadwell instruction support).

MORE DETAILED BENCHMARKS
--------------------------------------------------------------------------------

By default, each run within a benchmark overwrites the previous run to save
screen space, leaving only the overall average timing.

NOTE: If the terminal width is less than 185 characters, the output format may
be difficult to read. Either increase terminal window size (or reduce font
size), or pass `--single`.

To see every single run individually, pass `--single`.

EXAMPLE OUTPUT
--------------------------------------------------------------------------------

$ python3 testbench.py -i 10 500b 500 1000 2000 4000  # 100 iterations
:: Benchmarking NIKE
Nike Test # 10/10 | log(p) =  505, e =  391 | Success    10 | [Keygen] Avg:   16.00 ms /    52.86 MCy | [Derive] Avg:    7.02 ms /   23.15 MCy   | All exchanges succeeded!
Nike Test # 10/10 | log(p) =  509, e =  393 | Success    10 | [Keygen] Avg:   16.96 ms /    56.12 MCy | [Derive] Avg:    7.06 ms /   23.29 MCy   | All exchanges succeeded!
Nike Test # 10/10 | log(p) = 1008, e =  643 | Success    10 | [Keygen] Avg:   94.24 ms /   311.27 MCy | [Derive] Avg:   36.95 ms /  121.97 MCy   | All exchanges succeeded!
Nike Test # 10/10 | log(p) = 2032, e = 1155 | Success    10 | [Keygen] Avg:  534.16 ms /  1763.28 MCy | [Derive] Avg:  241.43 ms /  796.91 MCy   | All exchanges succeeded!
Nike Test # 10/10 | log(p) = 4090, e = 2184 | Success    10 | [Keygen] Avg: 4907.71 ms / 16195.62 MCy | [Derive] Avg: 1791.73 ms / 5912.01 MCy   | All exchanges succeeded!

:: Benchmarking group action (and testing for compatibility with PEGASIS)
Action Test #   10 | log(p) =  505, e =  390 | Success    10 | Avg:    7.25 ms /   23.96 cy   | All tests passed!
Action Test #   10 | log(p) =  509, e =  391 | Success    10 | Avg:    7.24 ms /   24.01 cy   | All tests passed!
Action Test #   10 | log(p) = 1008, e =  642 | Success    10 | Avg:   36.60 ms /  120.81 cy   | All tests passed!
Action Test #   10 | log(p) = 2032, e = 1155 | Success    10 | Avg:  242.15 ms /  799.26 cy   | All tests passed!
Action Test #   10 | log(p) = 4090, e = 2185 | Success    10 | Avg: 1798.99 ms / 5940.29 cy   | All tests passed!

:: Benchmarking norm equation
Normeq Test #   10/10 | log(p) =  505, e =  390 | Success    10 | Avg:    6.22 ms /   20.92 MCy   | All tests passed!
Normeq Test #   10/10 | log(p) =  509, e =  392 | Success    10 | Avg:   11.54 ms /   38.08 MCy   | All tests passed!
Normeq Test #   10/10 | log(p) = 1008, e =  642 | Success    10 | Avg:   83.71 ms /  276.35 MCy   | All tests passed!
Normeq Test #   10/10 | log(p) = 2032, e = 1155 | Success    10 | Avg:  236.45 ms /  780.19 MCy   | All tests passed!
Normeq Test #   10/10 | log(p) = 4090, e = 2185 | Success    10 | Avg: 5416.07 ms / 17890.70 MCy   | All tests passed!

$ python3 testbench.py -ski 5 500b  # 5 iterations, show every benchmark individually
:: Benchmarking NIKE
Nike Test #    1/5 | log(p) =  505, e =  391 | Success     1 | [Keygen] Avg:   21.25 ms /   70.94 MCy, This:   21.25 ms /   70.94 MCy | [Derive] Avg:    7.53 ms /   24.85 MCy, This:    7.53 ms /   24.85 MCy
Nike Test #    2/5 | log(p) =  505, e =  391 | Success     2 | [Keygen] Avg:   16.90 ms /   56.20 MCy, This:   12.55 ms /   41.47 MCy | [Derive] Avg:    7.28 ms /   24.02 MCy, This:    7.03 ms /   23.19 MCy
Nike Test #    3/5 | log(p) =  505, e =  391 | Success     3 | [Keygen] Avg:   18.15 ms /   60.15 MCy, This:   20.63 ms /   68.06 MCy | [Derive] Avg:    7.20 ms /   23.75 MCy, This:    7.04 ms /   23.21 MCy
Nike Test #    4/5 | log(p) =  505, e =  391 | Success     4 | [Keygen] Avg:   18.37 ms /   60.83 MCy, This:   19.05 ms /   62.84 MCy | [Derive] Avg:    7.16 ms /   23.60 MCy, This:    7.02 ms /   23.16 MCy
Nike Test #    5/5 | log(p) =  505, e =  391 | Success     5 | [Keygen] Avg:   17.42 ms /   57.65 MCy, This:   13.63 ms /   44.94 MCy | [Derive] Avg:    7.13 ms /   23.52 MCy, This:    7.03 ms /   23.17 MCy
All exchanges succeeded!

CYCLE COUNTS ON APPLE HARDWARE
--------------------------------------------------------------------------------

To see cycle counts on Apple's devices, this script must be executed with
elevated priviliges (i.e. with `sudo`).
(This issue has been observed by others
e.g. https://artifacts.iacr.org/tches/2022/a2/readme.html)

Some security can be reclaimed by running this script inside an ephemeral docker
container (that itself has elevated priviliges):

sudo docker run -it --rm ubuntu \
    bash -c "export DEBIAN_FRONTEND=noninteractive \
    && apt-get update -qqy \
    && apt-get install -qqy python3 gcc cmake libgmp-dev git \
    && git clone https://github.com/CORAL-nike/CORAL \
    && cd CORAL \
    && python3 testbench.py \
    ; echo 'Dropping into interactive shell...' \
    && echo 'Run more benchmarks with \`testbench.py\` or exit with \`exit\`' \
    && bash"

In any case, it is encouraged to carefully read and review all this code before
running it.
```

### Creating new action benchmark vectors

New benchmark vectors can be produced using `coral-sage/c_test_vectors.py`, e.g.

```
cd coral-sage
python3 c_test_vectors.py -p500 > /tmp/new_vectors_500.txt
```

This creates a new file `/tmp/new_vectors_500.txt` with the following three
lines:

1. A secret key
2. An input curve E
3. The action of the secret key on E

Create many vectors (e.g. 100) with

```
for i in {1..100}; do python3 c_test_vectors.py -p500 >> /tmp/new_vectors_500.txt; done
```

Note: To create new test vectors for the Broadwell prime, pass the option
`-p505` to `c_test_vectors.py`.

Test these vectors with

```
# cd back to root of git repository
cd ..
# Recompile project
cmake -S coral-c -B coral-c/build -DSQISIGN_BUILD_TYPE=ref -DCMAKE_BUILD_TYPE=release
make -j -C coral-c/build
cat /tmp/new_vectors_500.txt | ./coral-c/build/src/nike/ref/p_500/test/test_nike_action_p_500
```

The program `test_nike_action_p_500` will

1. Parse the secret key, input curve `E_in`, and output curve `E_out`
2. Compute the action of the secret key on the input curve
3. Compare the result `E_res` to `E_out`


### General usage

Compile the reference implementation with

```
cmake -S coral-c -B coral-c/build -DSQISIGN_BUILD_TYPE=ref -DCMAKE_BUILD_TYPE=release
make -j -C coral-c/build
```

Compile the Broadwell implementation with

```
cmake -S coral-c -B coral-c/build -DSQISIGN_BUILD_TYPE=broadwell -DCMAKE_BUILD_TYPE=release
make -j -C coral-c/build
```

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

