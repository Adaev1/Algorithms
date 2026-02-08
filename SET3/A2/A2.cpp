#include <iostream>
#include <random>
#include <iomanip>
#include <vector>
using namespace std;

void insertion_sort(vector<int> &a, int l, int r) {
    for (int i = l + 1; i < r; ++i) {
        int x = a[i];
        int j = i - 1;
        while (j >= l && a[j] > x) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = x;
    }
}

void merge_range(vector<int> &a, vector<int> &buf, int l, int m, int r) {
    int i = l, j = m, k = l;
    while (i < m && j < r) {
        if (a[i] <= a[j]) buf[k++] = a[i++];
        else buf[k++] = a[j++];
    }
    while (i < m) buf[k++] = a[i++];
    while (j < r) buf[k++] = a[j++];
    for (int p = l; p < r; ++p) a[p] = buf[p];
}

void merge_sort_std(vector<int> &a, vector<int> &buf, int l, int r) {
    if (r - l <= 1) return;
    int m = l + (r - l) / 2;
    merge_sort_std(a, buf, l, m);
    merge_sort_std(a, buf, m, r);
    merge_range(a, buf, l, m, r);
}

void merge_sort_hybrid(vector<int> &a, vector<int> &buf, int l, int r, int threshold) {
    int len = r - l;
    if (len <= 1) return;
    if (len <= threshold) {
        insertion_sort(a, l, r);
        return;
    }
    int m = l + len / 2;
    merge_sort_hybrid(a, buf, l, m, threshold);
    merge_sort_hybrid(a, buf, m, r, threshold);
    merge_range(a, buf, l, m, r);
}

class ArrayGenerator {
    int maxN;
    int minVal, maxVal;
    mt19937 rng;
    vector<int> baseRandom;
    vector<int> baseDesc;
    vector<int> baseAlmost;

public:
    ArrayGenerator(int maxN_, int minVal_, int maxVal_)
        : maxN(maxN_), minVal(minVal_), maxVal(maxVal_), rng(42) {
        baseRandom.resize(maxN);
        uniform_int_distribution<int> dist(minVal, maxVal);
        for (int i = 0; i < maxN; ++i) {
            baseRandom[i] = dist(rng);
        }
        vector<int> sorted = baseRandom;
        sort(sorted.begin(), sorted.end());
        baseDesc = sorted;
        reverse(baseDesc.begin(), baseDesc.end());
        baseAlmost = sorted;
        int swaps = maxN / 100;
        if (swaps < 1) swaps = 1;
        uniform_int_distribution<int> pos(0, maxN - 1);
        for (int k = 0; k < swaps; ++k) {
            int i = pos(rng);
            int j = pos(rng);
            if (i != j) swap(baseAlmost[i], baseAlmost[j]);
        }
    }

    vector<int> getRandom(int n) const {
        return vector<int>(baseRandom.begin(), baseRandom.begin() + n);
    }

    vector<int> getReversed(int n) const {
        return vector<int>(baseDesc.begin(), baseDesc.begin() + n);
    }

    vector<int> getAlmostSorted(int n) const {
        return vector<int>(baseAlmost.begin(), baseAlmost.begin() + n);
    }
};

class SortTester {
    ArrayGenerator &gen;
    int repeats;
    vector<int> thresholds;

public:
    SortTester(ArrayGenerator &g, int rep, const vector<int> &ths)
        : gen(g), repeats(rep), thresholds(ths) {
    }

    long long measureMerge(const vector<int> &base) const {
        using namespace chrono;
        int n = (int) base.size();
        long long total = 0;
        for (int r = 0; r < repeats; ++r) {
            vector<int> a = base;
            vector<int> buf(n);
            auto t0 = high_resolution_clock::now();
            merge_sort_std(a, buf, 0, n);
            auto t1 = high_resolution_clock::now();
            total += duration_cast<microseconds>(t1 - t0).count();
        }
        return total / repeats;
    }

    long long measureHybrid(const vector<int> &base, int threshold) const {
        using namespace chrono;
        int n = (int) base.size();
        long long total = 0;
        for (int r = 0; r < repeats; ++r) {
            vector<int> a = base;
            vector<int> buf(n);
            auto t0 = high_resolution_clock::now();
            merge_sort_hybrid(a, buf, 0, n, threshold);
            auto t1 = high_resolution_clock::now();
            total += duration_cast<microseconds>(t1 - t0).count();
        }
        return total / repeats;
    }

    void run() {
        cout << "#type size merge";
        for (int t: thresholds) cout << " hybrid_" << t;
        cout << "\n";
        const int Nmin = 500;
        const int Nmax = 100000;
        const int step = 100;
        for (int kind = 0; kind < 3; ++kind) {
            for (int n = Nmin; n <= Nmax; n += step) {
                vector<int> base;
                if (kind == 0) base = gen.getRandom(n);
                else if (kind == 1) base = gen.getReversed(n);
                else base = gen.getAlmostSorted(n);
                long long tMerge = measureMerge(base);
                cout << kind << " " << n << " " << tMerge;
                for (int thr: thresholds) {
                    long long tH = measureHybrid(base, thr);
                    cout << " " << tH;
                }
                cout << "\n";
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int maxN = 100000;
    int minVal = 0, maxVal = 10000;
    ArrayGenerator generator(maxN, minVal, maxVal);
    vector<int> ths = {5, 10, 20, 30, 50};
    SortTester tester(generator, 3, ths);
    tester.run();
    return 0;
}
