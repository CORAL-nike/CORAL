#!/usr/bin/env python3

from argparse import ArgumentParser
from multiprocessing import Event, Manager, Process, cpu_count
from queue import Empty
from signal import SIGINT, signal
from sys import stderr
from time import perf_counter_ns, sleep
import numpy as np


from sage.all import NumberField, factor, fundamental_discriminant, is_prime, isqrt, valuation, var, ceil, log

from ideal2d import sample_ideal2d
from utilities.class_number_formula import approx_class_number
import json

SENTINEL = None

import random

class RandomRange:
    def __init__(self, n, m):
        self.n = n
        self.m = m

    def __iter__(self):
        for _ in range(self.m):
            yield int(random.randrange(self.n))

def fillqueue(grexit, q_in, q_out, iterable, cores):
    for ctr, element in enumerate(iterable):
        if ctr % 2**12 == 0 and grexit.is_set():
            q_out.put(SENTINEL)
            return
        q_in.put(element)

    for _ in range(cores):
        q_in.put(SENTINEL)

    q_out.put(SENTINEL)


def sample(grexit, p, e, q_in, q_out):
    K = NumberField(name="pi", polynomial=var("x") ** 2 + p)
    order = K.order_of_conductor(2)
    w = K.gens()[0]
    started = False

    while not grexit.is_set():
        try:
            y = q_in.get(timeout=1)
        except Empty:
            continue

        if not started:
            started = True
            print('Worker started', file=stderr)

        if y is SENTINEL:
            break

        y = 2 * y + 1
        ideal = sample_ideal2d(p, e, stats=None,
                               try_y=y, bit_y=0,
                               only_prime_M=False,
                               postinit=False)

        if ideal is None:
            q_out.put((0, 0, 0, 0))

        else:
            # print('found ideal for y =', y, file=stderr)
            q, x, y, e = ideal
            ideal = order * q + order * (x % q + w * y)
            form = ideal.quadratic_form().reduced_form()
            q_out.put((y, form[0], form[1], form[2]))

            # case of -y
            # ideal = order * q + order * (x % q - w * y)
            # form = ideal.quadratic_form().reduced_form()
            # q_out.put((-y, form[0], form[1], form[2]))


    q_out.put(SENTINEL)


if __name__ == "__main__":
    parser = ArgumentParser(prog="Coral", description="", epilog="")

    parser.add_argument("-p", "--prime", type=int, default=32)
    parser.add_argument("-o", "--only-primes", action="store_true")
    parser.add_argument("-s", "--seed", type=int, default=1)
    parser.add_argument("-j", "--jobs", type=int, default=cpu_count() - 1)
    parser.add_argument("-e", "--exponent", type=int, default=0, help="Pass the exponent controlling the size of y samples")
    parser.add_argument("-w", "--write", type=bool, default=True)
    parser.add_argument("-r", "--random", action="store_true", help="Randomize the y samples instead of iterating sequentially")
    parser.add_argument("--hardcode", action="store_true")

    args = parser.parse_args()

    RANDOM = args.random

    # 2-adic valuation of p + 1 (essentially log(p) +- epsilon)
    p_2 = args.prime
    print("Finding prime", file=stderr)
    p = next(c * 2**p_2 - 1 for c in range(1, 2**12) if is_prime(c * 2**p_2 - 1))
    print(f"Found p = {factor(p + 1)} - 1", file=stderr)
    E = valuation(p + 1, 2) - 2
    if args.exponent > 0:
        E = isqrt(p).bit_length() + args.exponent + 2
        if args.hardcode:
            E = args.exponent

    size_y = 2 ** (E - 2) // isqrt(p) - 1

    class_number = approx_class_number(fundamental_discriminant(-p))

    manager = Manager()
    q_in = manager.Queue(maxsize=2**32)
    q_out = manager.Queue()
    grexit = Event()

    processes = []
    if not RANDOM:
        m = size_y
        processes += [Process(target=fillqueue, args=(grexit, q_in, q_out, range(size_y), args.jobs))]
    else:
        print("Random sampling mode enabled", file=stderr)
        # m = size_y + H_size_y
        m = size_y * ceil(log(size_y, 2.))
        # if args.hardcode:
        #     m = 2**(20)
        processes += [Process(target=fillqueue, args=(grexit, q_in, q_out, RandomRange(size_y, m), args.jobs))]
        # processes += [Process(target=fillqueue, args=(grexit, q_in, q_out, np.random.randint(0, size_y, size=m), args.jobs))]
    processes += [Process(target=sample, args=(grexit, p, E, q_in, q_out)) for _ in range(args.jobs)]

    print(f"Using {m} samples of y<{size_y} e = {E} (bits = {m.bit_length()})", file=stderr)

    processes[0].start()

    sleep(1)  # Give the queue filler a moment to start

    for process in processes[1:]:
        process.start()

    sleep(5)  # Give the workers a moment to start
    print('Processes started')

    fails = 0
    successes = 0
    collisions = 0
    classes = dict()
    processes_done = 0
    y_samples = 0

    def stats():
        if grexit.is_set():
            print("(Finishing up...) ", end="")

        out = []
        out += [f"Throughput {y_samples / (perf_counter_ns() - start) * 10**9:.2f}/s"]
        # out += [f"Fails {fails}"]
        out += [f"Successes: {successes}"]
        out += [f"Percentage: {float(y_samples) / float(m) * 100:.2f}%"]
        out += [f"Collisions: {collisions}"]
        out += [f"Percentage: {float(y_samples) / m * 100:.2f}%"]
        out += [f"Classes: {len(classes)}/{class_number}"]
        out += [f"form queue: {q_out.qsize()}"]
        out += [f"y queue: {q_in.qsize()}"]

        print(", ".join(out))

    # Listen for ctrl-c to gracefully exit
    signal(SIGINT, lambda _, __: grexit.set())

    start = perf_counter_ns()

    m_one_percent = m // 100

    incremental_data = {}

    while processes_done < args.jobs + 1:
        if y_samples % m_one_percent == 0:
            stats()
            if RANDOM: 
                incremental_data[int(y_samples)] = {
                        "fails": fails,
                        "successes": successes,
                        "collisions": collisions,
                        "classes": len(classes),
                        "first_collision": False,
                        }

        try:
            form = q_out.get(timeout=1)
        except Empty:
            continue

        y_samples += 1

        if form is SENTINEL:
            processes_done += 1

        else:
            y, a, b, c = form

            # hnf = (a, b, c)
            hnf = f"({a}, {b}, {c})"

            if a == b == c == 0:
                fails += 1

            else:
                successes += 1
                try:
                    seen = classes[hnf]

                    if seen and y not in seen:
                        collisions += 1

                        if collisions == 1:
                            incremental_data[int(y_samples)] = {
                                    "fails": fails,
                                    "successes": successes,
                                    "collisions": collisions,
                                    "classes": len(classes),
                                    "first_collision": True,
                                    }

                        # print(
                        #     f"Collision {collisions}: Class ({a}, {b}, {c}) is obtained by y = ",
                        #     classes[hnf] + [y],
                        # )

                except KeyError:
                    seen = []

                classes[hnf] = seen + [y]
    stats()


    if args.write:
        import time
        date = time.strftime("%Y-%m-%d_%H-%M-%S", time.gmtime())
        pexp = valuation(p + 1, 2)
        c = (p + 1) // 2**pexp
        output_name = f"data/sampling_distribution-p_{pexp}_{c}-e_{E}-{date}.json"

        data = {
            "prime": int(p),
            "exponent": int(E),
            "fails": int(fails),
            "successes": int(successes),
            "collisions": int(collisions),
            "primality_condition": False, # args.only_primes, to change
            "classes": classes,
        }
        with open(output_name, "w") as f:
            json.dump(data, f, indent=2)
        print(f"Written output to {output_name}")

    histogram = dict()

    for v in classes.values():
        histogram[len(v)] = histogram.get(len(v), 0) + 1

    print('Size of sampled classes:', len(classes))
    print(f'Expected reps          : {fails / successes:.2f}')

    print("Collision histogram")
    for k, v in sorted(histogram.items(), key=lambda kv: kv[0]):
        print("  ", k, v)

    if args.write:
        import time
        date = time.strftime("%Y-%m-%d_%H-%M-%S", time.gmtime())
        pexp = valuation(p + 1, 2)
        c = (p + 1) // 2**pexp
        if not RANDOM:
            output_name = f"data/sampling_distribution-p_{pexp}_{c}-e_{E}-{date}.json"
        else:
            output_name = f"data/rdist-p_{pexp}_{c}-e_{E}-{date}.json"
            # polishing classes counting lists 
            for k, v in classes.items():
                classes[k] = { x : v.count(x) for x in set(v) }


        data = {
            "prime": int(p),
            "exponent": int(E),
            "fails": int(fails),
            "successes": int(successes),
            "collisions": int(collisions),
            "primality_condition": False, # args.only_primes, to change
            "classes": classes,
        }
        if args.random:
            data["sample"] = "randomized over y"
            data["incremental_data"] = incremental_data
        else:
            data["sample"] = "sequential over y"

        with open(output_name, "w") as f:
            json.dump(data, f, indent=2)
        print(f"Written output to {output_name}")


    print("Done")

    if grexit.is_set():
        print("Exited early")

    for process in processes:
        process.join()

    import sys
    # return 0 if no collisions
    if len(histogram) == 1:
        sys.exit(0)
    else:
        sys.exit(1)
