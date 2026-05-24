import sys

sys.setrecursionlimit(1_000_000)


class Counter:
    __slots__ = ("v",)

    def __init__(self):
        self.v = 0


def _cmp_str(a, b, ctr):
    la, lb = len(a), len(b)
    i = 0
    m = la if la < lb else lb
    while i < m:
        ctr.v += 1
        if a[i] != b[i]:
            return -1 if a[i] < b[i] else 1
        i += 1
    if la == lb:
        return 0
    return -1 if la < lb else 1


def std_quicksort(arr):
    a = list(arr)
    ctr = Counter()

    def qs(lo, hi):
        if hi - lo <= 1:
            return
        mid = (lo + hi) // 2
        pivot = a[mid]
        a[mid], a[hi - 1] = a[hi - 1], a[mid]
        store = lo
        for i in range(lo, hi - 1):
            if _cmp_str(a[i], pivot, ctr) < 0:
                a[store], a[i] = a[i], a[store]
                store += 1
        a[store], a[hi - 1] = a[hi - 1], a[store]
        qs(lo, store)
        qs(store + 1, hi)

    qs(0, len(a))
    return a, ctr.v


def std_mergesort(arr):
    a = list(arr)
    ctr = Counter()
    tmp = [None] * len(a)

    def merge(lo, mid, hi):
        for k in range(lo, hi):
            tmp[k] = a[k]
        i, j = lo, mid
        for k in range(lo, hi):
            if i >= mid:
                a[k] = tmp[j]; j += 1
            elif j >= hi:
                a[k] = tmp[i]; i += 1
            elif _cmp_str(tmp[i], tmp[j], ctr) <= 0:
                a[k] = tmp[i]; i += 1
            else:
                a[k] = tmp[j]; j += 1

    def ms(lo, hi):
        if hi - lo <= 1:
            return
        mid = (lo + hi) // 2
        ms(lo, mid)
        ms(mid, hi)
        merge(lo, mid, hi)

    ms(0, len(a))
    return a, ctr.v


def _char_at(s, d, ctr):
    ctr.v += 1
    if d < len(s):
        return ord(s[d])
    return -1


def string_quicksort(arr):
    a = list(arr)
    ctr = Counter()

    def qs(lo, hi, d):
        if hi - lo <= 1:
            return
        lt, gt = lo, hi - 1
        v = _char_at(a[lo], d, ctr)
        i = lo + 1
        while i <= gt:
            t = _char_at(a[i], d, ctr)
            if t < v:
                a[lt], a[i] = a[i], a[lt]
                lt += 1; i += 1
            elif t > v:
                a[i], a[gt] = a[gt], a[i]
                gt -= 1
            else:
                i += 1
        qs(lo, lt, d)
        if v >= 0:
            qs(lt, gt + 1, d + 1)
        qs(gt + 1, hi, d)

    qs(0, len(a), 0)
    return a, ctr.v


def string_mergesort(arr):
    a = list(arr)
    n = len(a)
    ctr = Counter()
    lcp = [0] * n
    tmp = [None] * n
    tlcp = [0] * n

    def lcp_compare(x, y, k):
        lx, ly = len(x), len(y)
        i = k
        m = lx if lx < ly else ly
        while i < m:
            ctr.v += 1
            if x[i] != y[i]:
                return (-1 if x[i] < y[i] else 1), i
            i += 1
        if lx == ly:
            return 0, i
        return (-1 if lx < ly else 1), i

    def merge(lo, mid, hi):
        i, j, k = lo, mid, lo
        ll, lr = 0, 0
        while i < mid and j < hi:
            if ll > lr:
                cmp, nl = -1, lr
            elif ll < lr:
                cmp, nl = 1, ll
            else:
                cmp, nl = lcp_compare(a[i], a[j], ll)
            if cmp <= 0:
                tmp[k] = a[i]; tlcp[k] = ll
                i += 1
                ll = lcp[i] if i < mid else 0
                lr = nl
            else:
                tmp[k] = a[j]; tlcp[k] = lr
                j += 1
                lr = lcp[j] if j < hi else 0
                ll = nl
            k += 1
        while i < mid:
            tmp[k] = a[i]; tlcp[k] = ll
            i += 1
            ll = lcp[i] if i < mid else 0
            k += 1
        while j < hi:
            tmp[k] = a[j]; tlcp[k] = lr
            j += 1
            lr = lcp[j] if j < hi else 0
            k += 1
        for t in range(lo, hi):
            a[t] = tmp[t]
            lcp[t] = tlcp[t]

    def ms(lo, hi):
        if hi - lo <= 1:
            return
        mid = (lo + hi) // 2
        ms(lo, mid)
        ms(mid, hi)
        merge(lo, mid, hi)

    ms(0, n)
    return a, ctr.v


R = 256


def msd_radix_sort(arr):
    a = list(arr)
    n = len(a)
    ctr = Counter()
    aux = [None] * n

    def msd(lo, hi, d):
        if hi - lo <= 1:
            return
        cnt = [0] * (R + 2)
        for i in range(lo, hi):
            c = _char_at(a[i], d, ctr)
            cnt[c + 2] += 1
        for r in range(R + 1):
            cnt[r + 1] += cnt[r]
        gs = cnt[:]
        for i in range(lo, hi):
            c = _char_at(a[i], d, ctr)
            aux[lo + cnt[c + 1]] = a[i]
            cnt[c + 1] += 1
        for i in range(lo, hi):
            a[i] = aux[i]
        for c in range(R):
            sl = lo + gs[c + 1]
            sh = lo + gs[c + 2]
            if sh - sl > 1:
                msd(sl, sh, d + 1)

    msd(0, n, 0)
    return a, ctr.v


ALPHABET_SIZE = 74


def msd_radix_quick_sort(arr):
    a = list(arr)
    n = len(a)
    ctr = Counter()
    aux = [None] * n

    def tqs(lo, hi, d):
        if hi - lo <= 1:
            return
        lt, gt = lo, hi - 1
        v = _char_at(a[lo], d, ctr)
        i = lo + 1
        while i <= gt:
            t = _char_at(a[i], d, ctr)
            if t < v:
                a[lt], a[i] = a[i], a[lt]
                lt += 1; i += 1
            elif t > v:
                a[i], a[gt] = a[gt], a[i]
                gt -= 1
            else:
                i += 1
        tqs(lo, lt, d)
        if v >= 0:
            tqs(lt, gt + 1, d + 1)
        tqs(gt + 1, hi, d)

    def msd(lo, hi, d):
        if hi - lo <= 1:
            return
        if hi - lo < ALPHABET_SIZE:
            tqs(lo, hi, d)
            return
        cnt = [0] * (R + 2)
        for i in range(lo, hi):
            c = _char_at(a[i], d, ctr)
            cnt[c + 2] += 1
        for r in range(R + 1):
            cnt[r + 1] += cnt[r]
        gs = cnt[:]
        for i in range(lo, hi):
            c = _char_at(a[i], d, ctr)
            aux[lo + cnt[c + 1]] = a[i]
            cnt[c + 1] += 1
        for i in range(lo, hi):
            a[i] = aux[i]
        for c in range(R):
            sl = lo + gs[c + 1]
            sh = lo + gs[c + 2]
            if sh - sl > 1:
                msd(sl, sh, d + 1)

    msd(0, n, 0)
    return a, ctr.v


ALL_ALGORITHMS = [
    ("Std QuickSort", std_quicksort),
    ("Std MergeSort", std_mergesort),
    ("String QuickSort", string_quicksort),
    ("String MergeSort", string_mergesort),
    ("MSD Radix Sort", msd_radix_sort),
    ("MSD Radix + QuickSort", msd_radix_quick_sort),
]
