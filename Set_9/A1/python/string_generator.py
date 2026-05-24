import random


class StringGenerator:
    SPECIAL = "!@#$%^&*()_+"
    ALPHABET = (
        "".join(chr(c) for c in range(ord("A"), ord("Z") + 1))
        + "".join(chr(c) for c in range(ord("a"), ord("z") + 1))
        + "".join(chr(c) for c in range(ord("0"), ord("9") + 1))
        + SPECIAL
    )

    def __init__(self, seed=42, min_len=10, max_len=200):
        assert len(self.ALPHABET) == 74
        self.rng = random.Random(seed)
        self.min_len = min_len
        self.max_len = max_len

    def _one(self):
        L = self.rng.randint(self.min_len, self.max_len)
        return "".join(self.rng.choice(self.ALPHABET) for _ in range(L))

    def random(self, n):
        return [self._one() for _ in range(n)]

    def sorted(self, n):
        a = self.random(n)
        a.sort()
        return a

    def reverse_sorted(self, n):
        a = self.random(n)
        a.sort(reverse=True)
        return a

    def almost_sorted(self, n, swaps=None):
        a = self.sorted(n)
        if n < 2:
            return a
        if swaps is None:
            swaps = max(1, n // 100)
        for _ in range(swaps):
            i = self.rng.randrange(n)
            j = self.rng.randrange(n)
            a[i], a[j] = a[j], a[i]
        return a
