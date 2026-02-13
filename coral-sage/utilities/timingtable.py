#!/usr/bin/env python3

from statistics import mean, stdev
from time import perf_counter_ns, sleep

from .table import strtable


def _stdev(data):
    return stdev(data) if len(data) > 1 else -1


class TimingTable:
    def __init__(self, counters=(), counter_names=(), precision=None, round=None):
        if precision == "ns":
            divisor = 1
            self.header = "ns"
        elif precision == "us":
            divisor = 10**3
            self.header = "us"
        elif precision == "ms":
            divisor = 10**6
            self.header = "ms"
        else:
            divisor = 10**9
            self.header = "s"

        def now():
            return [perf_counter_ns() / divisor] + [counter() for counter in counters]

        if round is not None:
            self.round = round
        else:
            self.round = 1

        # Make mutable
        self.counter = list(counters)
        self.counter_names = list(counter_names)
        self.current = 0
        self.events = []
        self.now = now
        self.notime = set()
        self.nondiff_events = {}

    def init(self):
        self.prev = self.now()
        self.current = 0

    def update(self, event, nondiff=None, value=None):
        now = self.now()

        if nondiff is not None:
            self.nondiff_events[event] = self.nondiff_events.get(event, []) + [nondiff]
            return

        if value is None:
            # Get diff from prev
            if len(self.events) <= self.current:
                self.events += [[event, [[now[i] - self.prev[i] for i in range(len(now))]]]]
            else:
                assert event == self.events[self.current][0], "Different usage between loops"
                self.events[self.current][1] += [[now[i] - self.prev[i] for i in range(len(now))]]

            self.prev = self.now()
        else:
            # Set value manually
            if len(self.events) <= self.current:
                self.events += [[event, [value]]]
            else:
                assert event == self.events[self.current][0], "Different usage between loops"
                self.events[self.current][1] += [value]

        self.current += 1

    def end(self):
        pass

    def table(self):
        if len(self.events) == 0:
            return "No stats collected yet"

        n_counters = len(self.now())

        # fmt: off
        header_table = []
        header_table += [
            [""]
            + [""]
            + ([""] + ["Mean"]) * n_counters
            + [""] + ["Min"] + ["-"] * (n_counters - 1)
            + [""] + ["Max"] + ["-"] * (n_counters - 1)
            + [""] + ["Stdev"] + ["-"] * (n_counters - 1)
        ]
        # fmt: on

        row = [""] + [""]
        for header in [self.header] + self.counter_names:
            row += ["%", f"{header}"]
        row += ([""] + [f"{header}" for header in [self.header] + self.counter_names]) * 3

        diff_table = [row]

        run_totals = [
            [sum([event_data[i][ctr] for _, event_data in self.events]) for ctr in range(n_counters)]
            for i in range(len(self.events[0][1]))
        ]

        mean_totals = [mean([datum[i] for datum in run_totals]) for i in range(n_counters)]

        for event_name, event_data in self.events + [["Total", run_totals]]:
            _means = [mean([datum[i] for datum in event_data]) for i in range(n_counters)]
            row = []
            row += [f"{event_name}"]
            row += ["|"]
            for ctr in range(n_counters):
                row += ["n/a" if mean_totals[ctr] == 0 else f"{_means[ctr] / mean_totals[ctr] * 100:.{self.round}f}"]
                row += [f"{_means[ctr]:.{self.round}f}"]
            row += ["|"] + [f"{min([datum[i] for datum in event_data]):.{self.round}f}" for i in range(n_counters)]
            row += ["|"] + [f"{max([datum[i] for datum in event_data]):.{self.round}f}" for i in range(n_counters)]
            row += ["|"] + [f"{_stdev([datum[i] for datum in event_data]):.{self.round}f}" for i in range(n_counters)]
            diff_table += [row]

        nondiff_table = []
        for event_name, event_data in self.nondiff_events.items():
            row = []
            row += [f"{event_name}"]
            row += ["|"]
            row += [""] + [f"{mean(event_data):.{self.round}f}"]
            for _ in range(n_counters - 1):
                row += [""]
                row += [""]
            row += ["|"] + [f"{min(event_data):.{self.round}f}"] + [""] * (n_counters - 1)
            row += ["|"] + [f"{max(event_data):.{self.round}f}"] + [""] * (n_counters - 1)
            row += ["|"] + [f"{_stdev(event_data):.{self.round}f}"] + [""] * (n_counters - 1)
            nondiff_table += [row]

        table = (
            header_table
            + [["" for _ in range(len(header_table[0]))]]
            + nondiff_table
            + [["" for _ in range(len(header_table[0]))]]
            + diff_table
        )

        return table

    def __repr__(self):
        if len(self.events) == 0:
            return "No stats collected yet"
        return strtable(self.table())


if __name__ == "__main__":

    def counter1():
        return perf_counter_ns() / 10**6

    def counter2():
        return perf_counter_ns() / 10**9

    tt = TimingTable(precision="us", round=1, counters=[counter1, counter2], counter_names=["ms", "s"])

    for _ in range(7):
        tt.init()
        sleep(0.01)
        tt.update("Event 1")
        sleep(0.02)
        tt.update("Event 2")
        sleep(0.03)
        tt.update("Event 3")
        sleep(0.04)
        tt.update("Samples", nondiff=_)
        tt.update("Event 4")
        tt.end()

    print(tt)
