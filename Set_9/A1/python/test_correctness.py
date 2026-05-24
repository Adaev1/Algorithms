from string_generator import StringGenerator
from algorithms import ALL_ALGORITHMS


def check_all():
    gen = StringGenerator(seed=42)
    cases = [
        ("random",         gen.random),
        ("sorted",         gen.sorted),
        ("reverse_sorted", gen.reverse_sorted),
        ("almost_sorted",  gen.almost_sorted),
    ]
    for kind, factory in cases:
        for n in [0, 1, 2, 10, 100, 500, 1500]:
            arr = factory(n)
            expected = sorted(arr)
            for name, fn in ALL_ALGORITHMS:
                result, comparisons = fn(arr)
                if result != expected:
                    print(f"FAIL: {name} kind={kind} n={n}")
                    return False
        print(f"OK: {kind}")
    return True


if __name__ == "__main__":
    ok = check_all()
    print("ALL OK" if ok else "FAILED")
