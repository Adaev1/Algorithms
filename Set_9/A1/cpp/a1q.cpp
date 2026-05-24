#include <iostream>
#include <vector>
#include <string>

using namespace std;

int charAt(const string& s, int d) {
    if (d < s.length()) return s[d];
    return -1;
}

void stringQuickSort(vector<string>& a, int lo, int hi, int d) {
    if (hi <= lo) return;
    int lt = lo, gt = hi;
    int v = charAt(a[lo], d);
    int i = lo + 1;
    while (i <= gt) {
        int t = charAt(a[i], d);
        if (t < v) swap(a[lt++], a[i++]);
        else if (t > v) swap(a[i], a[gt--]);
        else i++;
    }
    stringQuickSort(a, lo, lt - 1, d);
    if (v >= 0) stringQuickSort(a, lt, gt, d + 1);
    stringQuickSort(a, gt + 1, hi, d);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (!(cin >> n)) return 0;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    stringQuickSort(a, 0, n - 1, 0);

    for (const string& s : a) cout << s << "\n";
    return 0;
}
