#include <iostream>
#include <vector>
#include <string>

using namespace std;

int lcpCompare(const string& a, const string& b, int k, int& outLcp) {
    int la = a.length(), lb = b.length();
    int i = k;
    while (i < la && i < lb && a[i] == b[i]) ++i;
    outLcp = i;
    if (i == la && i == lb) return 0;
    if (i == la) return -1;
    if (i == lb) return 1;
    return (unsigned char)a[i] < (unsigned char)b[i] ? -1 : 1;
}

void merge(vector<string>& a, vector<int>& lcp,
           vector<string>& tmp, vector<int>& tlcp,
           int lo, int mid, int hi) {
    int i = lo, j = mid, k = lo;
    int ll = 0, lr = 0;
    while (i < mid && j < hi) {
        int cmp, nl;
        if (ll > lr) { cmp = -1; nl = lr; }
        else if (ll < lr) { cmp = 1; nl = ll; }
        else cmp = lcpCompare(a[i], a[j], ll, nl);

        if (cmp <= 0) {
            tmp[k] = std::move(a[i]);
            tlcp[k] = ll;
            ++i;
            ll = (i < mid) ? lcp[i] : 0;
            lr = nl;
        } else {
            tmp[k] = std::move(a[j]);
            tlcp[k] = lr;
            ++j;
            lr = (j < hi) ? lcp[j] : 0;
            ll = nl;
        }
        ++k;
    }
    while (i < mid) {
        tmp[k] = std::move(a[i]);
        tlcp[k] = ll;
        ++i;
        ll = (i < mid) ? lcp[i] : 0;
        ++k;
    }
    while (j < hi) {
        tmp[k] = std::move(a[j]);
        tlcp[k] = lr;
        ++j;
        lr = (j < hi) ? lcp[j] : 0;
        ++k;
    }
    for (int t = lo; t < hi; ++t) {
        a[t] = std::move(tmp[t]);
        lcp[t] = tlcp[t];
    }
}

void stringMergeSort(vector<string>& a, vector<int>& lcp,
                     vector<string>& tmp, vector<int>& tlcp,
                     int lo, int hi) {
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    stringMergeSort(a, lcp, tmp, tlcp, lo, mid);
    stringMergeSort(a, lcp, tmp, tlcp, mid, hi);
    merge(a, lcp, tmp, tlcp, lo, mid, hi);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (!(cin >> n)) return 0;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<int> lcp(n, 0);
    vector<string> tmp(n);
    vector<int> tlcp(n, 0);
    stringMergeSort(a, lcp, tmp, tlcp, 0, n);

    for (const string& s : a) cout << s << "\n";
    return 0;
}
