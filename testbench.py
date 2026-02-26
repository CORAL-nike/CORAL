from argparse import ArgumentParser, RawDescriptionHelpFormatter
from textwrap import dedent
from subprocess import run, PIPE, STDOUT
from sys import exit, stdout
from types import SimpleNamespace
from platform import machine


def _run(command, *args, **kwargs):


    try:
        result = run(command, text=True, *args, **kwargs)

        try:
            result.check_returncode()

        except Exception:
            pass
            if result.stdout:
                print(result.stdout)

            if result.stderr:
                print(result.stderr)

        if result.stdout:
            return result.stdout.strip('\n').split('\n')

    except FileNotFoundError:
        pass


def compile(param):
    print(f"Compiling for parameter p = {param.name} ('{param.build_type}' build)...", end="")
    stdout.flush()
    _run(f"cmake -S coral-c -B coral-c/build.{param.build_type} -DSQISIGN_BUILD_TYPE={param.build_type} -DCMAKE_BUILD_TYPE=release".split(), stdout=PIPE, stderr=STDOUT)
    _run(f"make -j -C coral-c/build.{param.build_type}".split(), stdout=PIPE, stderr=STDOUT)


def bench_nike(param, iterations, extra_args):
    compile(param)
    extra_args += [f"--iterations={iterations}"]
    _run([f"./coral-c/build.{param.build_type}/src/nike/ref/{param.level}/test/test_nike_{param.level}"] + extra_args)


def bench_action(param, iterations, extra_args):
    compile(param)

    if iterations > 1000:
        print("[WARNING] Only have 1000 pre-computed test vectors. Reducing iteration count to 1000")
        iterations = 1000

    extra_args += [f"--iterations={iterations}"]

    with open(f"coral-c/src/precomp/ref/{param.level}/nike_action_benchmark_vectors.txt") as f:
        test_vectors = f.read()

    _run([f"./coral-c/build.{param.build_type}/src/nike/ref/{param.level}/test/test_nike_action_{param.level}"] + extra_args, input=test_vectors)


def bench_normeq(param, iterations, extra_args):
    compile(param)

    extra_args += ["--test", f"--iterations={iterations}"]
    _run([f"./coral-c/build.{param.build_type}/src/nike/ref/{param.level}/test/bench_normeq_{param.level}", f"--bitsize={param.e}"] + extra_args)


if __name__ == "__main__":
    params = [
        SimpleNamespace(name="500b", level="lvl5", build_type="broadwell", e=390),
        SimpleNamespace(name="500", level="p_500", build_type="ref", e=392),
        SimpleNamespace(name="1000", level="p_1000", build_type="ref", e=642),
        SimpleNamespace(name="2000", level="p_2000", build_type="ref", e=1155),
        SimpleNamespace(name="4000", level="p_4000", build_type="ref", e=2185),
    ]

    params = {param.name: param for param in params}

    description = dedent("""
    CORAL Benchmarking/Test Script
    --------------------------------------------------------------------------------

    Without any options, this script computes 100 iterations of all benchmarks
    for all primes.

    Broadwell benchmarks will be automatically attempted if the machine is
    detected to have an `x86` architecture (there may be false positives).

    --------------------------------------------------------------------------------
    """)

    epilog= dedent(f"""

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

      * {list(params.keys())}

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

    sudo docker run -it --rm ubuntu \\
        bash -c "export DEBIAN_FRONTEND=noninteractive \\
        && apt-get update -qqy \\
        && apt-get install -qqy python3 gcc cmake libgmp-dev git \\
        && git clone https://github.com/CORAL-nike/CORAL \\
        && cd CORAL \\
        && python3 testbench.py \\
        ; echo 'Dropping into interactive shell...' \\
        && echo 'Run more benchmarks with \\`testbench.py\\` or exit with \\`exit\\`' \\
        && bash"

    In any case, it is encouraged to carefully read and review all this code before
    running it.
    """)

    parser = ArgumentParser(
                prog="testbench.py",
                description=description,
                epilog=epilog,
                formatter_class=RawDescriptionHelpFormatter
            )

    parser.add_argument("-k", "--nike", action="store_true", help="Perform key-exchange benchmarks")
    parser.add_argument("-a", "--action", action="store_true", help="Perform action benchmarks")
    parser.add_argument("-n", "--normeq", action="store_true", help="Perform norm equation benchmarks")
    parser.add_argument("-i", "--iterations", type=int, default=100, help="Number of iterations")
    parser.add_argument("-s", "--single", action="store_true", default=False, help="Show every single run (not just average)")
    parser.add_argument("params", nargs="*")

    args = parser.parse_args()

    # Cannot use `choices=` and `default=` in python 3.9
    for param in args.params:
        if param not in params.keys():
            print(f"[ERROR] '{param}' not a supported parameter. Choose subset of {list(params.keys())}")
            exit(1)

    default = []

    if machine() == "x86_64":
        default += ["500b"]

    default += ["500", "1000", "2000", "4000"]

    args.params = args.params or default

    if not (args.action or args.nike or args.normeq):
        args.action = args.nike = args.normeq = True

    extra_args = []

    if args.single:
        extra_args += ["--single"]

    if args.nike:
        print(":: Benchmarking NIKE")
        for param in args.params:
            bench_nike(params[param], args.iterations, extra_args)

    if args.action:
        if args.nike:
            print()
        print(":: Benchmarking group action (and testing for compatibility with PEGASIS)")
        for param in args.params:
            bench_action(params[param], args.iterations, extra_args)

    if args.normeq:
        if args.nike or args.action:
            print()
        print(":: Benchmarking norm equation")
        for param in args.params:
            bench_normeq(params[param], args.iterations, extra_args)
