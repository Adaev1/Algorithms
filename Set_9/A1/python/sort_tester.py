import time
import json
from string_generator import StringGenerator
from algorithms import ALL_ALGORITHMS


class StringSortTester:
    def __init__(self, repeats=3, seed=42):
        self.repeats = repeats
        self.seed = seed

    def measure(self, fn, arr):
        best_time = float("inf")
        comparisons = 0
        for _ in range(self.repeats):
            t0 = time.perf_counter()
            _, comparisons = fn(arr)
            dt = time.perf_counter() - t0
            if dt < best_time:
                best_time = dt
        return best_time, comparisons

    def run(self, ns, array_kind):
        results = {name: {"time_ms": [], "comparisons": []} for name, _ in ALL_ALGORITHMS}
        results["_ns"] = list(ns)
        for n in ns:
            gen = StringGenerator(seed=self.seed)
            factory = getattr(gen, array_kind)
            arr = factory(n)
            for name, fn in ALL_ALGORITHMS:
                t, c = self.measure(fn, arr)
                results[name]["time_ms"].append(t * 1000.0)
                results[name]["comparisons"].append(c)
            print(f"  n={n:5d} done")
        return results


def main():
    ns = list(range(100, 3001, 100))
    tester = StringSortTester(repeats=3)
    out = {}
    for kind in ("random", "almost_sorted", "reverse_sorted"):
        print(f"\n=== {kind} ===")
        out[kind] = tester.run(ns, kind)
    with open("../results/measurements.json", "w") as f:
        json.dump(out, f)
    print("\nSaved to ../results/measurements.json")


if __name__ == "__main__":
    main()
