#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int R = 256;

int charAt(const string& s, int d) {
    if (d < s.length()) return (unsigned char)s[d];
    return -1;
}

void msd(vector<string>& a, vector<string>& aux, int lo, int hi, int d) {
    if (hi - lo <= 1) return;

    vector<int> cnt(R + 2, 0);
    for (int i = lo; i < hi; ++i) {
        int c = charAt(a[i], d);
        ++cnt[c + 2];
    }
    for (int r = 0; r < R + 1; ++r) cnt[r + 1] += cnt[r];
    vector<int> gs(cnt);

    for (int i = lo; i < hi; ++i) {
        int c = charAt(a[i], d);
        aux[lo + cnt[c + 1]++] = std::move(a[i]);
    }
    for (int i = lo; i < hi; ++i) a[i] = std::move(aux[i]);

    for (int c = 0; c < R; ++c) {
        int sl = lo + gs[c + 1];
        int sh = lo + gs[c + 2];
        if (sh - sl > 1) msd(a, aux, sl, sh, d + 1);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (!(cin >> n)) return 0;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<string> aux(n);
    msd(a, aux, 0, n, 0);

    for (const string& s : a) cout << s << "\n";
    return 0;
}
