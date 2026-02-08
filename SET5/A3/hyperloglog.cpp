#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <random>
#include <cmath>

using namespace std;
typedef unsigned long long ull;

class RandomStreamGen {
    mt19937_64 rng;
    string alphabet;
public:
    RandomStreamGen(ull seed)
        : rng(seed),
          alphabet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-") {}

    string next() {
        uniform_int_distribution<int> len_dist(1, 30);
        uniform_int_distribution<int> ch_dist(0, alphabet.size() - 1);
        int len = len_dist(rng);
        string s;
        s.reserve(len);
        for (int i = 0; i < len; i++)
            s.push_back(alphabet[ch_dist(rng)]);
        return s;
    }
};

class HashFuncGen {
    ull seed;
public:
    HashFuncGen(ull s) : seed(s) {}

    uint32_t operator()(const string& s) const {
        uint32_t h = 2166136261u;
        for (unsigned char c : s) {
            h ^= c;
            h *= 16777619u;
        }
        h ^= (uint32_t)(seed & 0xFFFFFFFF);
        h ^= h >> 16;
        h *= 0x45d9f3bu;
        h ^= h >> 16;
        return h;
    }
};

class HyperLogLog {
    int B, m;
    vector<uint8_t> regs;
    HashFuncGen hash;

    double alpha_m() const {
        if (m == 16) return 0.673;
        if (m == 32) return 0.697;
        if (m == 64) return 0.709;
        return 0.7213 / (1.0 + 1.079 / (double)m);
    }

public:
    HyperLogLog(int B_, ull seed) : B(B_), m(1 << B_), regs(1 << B_, 0), hash(seed) {}

    void add(const string& s) {
        uint32_t x = hash(s);
        int idx = x >> (32 - B);
        uint32_t w = x << B;
        int rho = 1;
        while ((w & (1u << 31)) == 0 && rho <= 32 - B) {
            rho++;
            w <<= 1;
        }
        if (rho > regs[idx]) regs[idx] = rho;
    }

    double estimate() const {
        double am = alpha_m();
        double sum = 0;
        int zeros = 0;
        for (int i = 0; i < m; i++) {
            sum += ldexp(1.0, -regs[i]);
            if (regs[i] == 0) zeros++;
        }
        double E = am * m * m / sum;
        if (E <= 2.5 * m && zeros > 0)
            E = m * log((double)m / zeros);
        return E;
    }

    int memory_bytes() const { return m; }
};

class HyperLogLogPlus {
    int B, m;
    vector<uint8_t> regs;
    HashFuncGen hash;

    double alpha_m() const {
        if (m == 16) return 0.673;
        if (m == 32) return 0.697;
        if (m == 64) return 0.709;
        return 0.7213 / (1.0 + 1.079 / (double)m);
    }

    double bias_correction(double E) const {
        double ratio = E / m;
        if (ratio < 5.0)
            return E * (1.0 - 0.005 * (5.0 - ratio));
        return E;
    }

public:
    HyperLogLogPlus(int B_, ull seed) : B(B_), m(1 << B_), regs(1 << B_, 0), hash(seed) {}

    void add(const string& s) {
        uint32_t x = hash(s);
        int idx = x >> (32 - B);
        uint32_t w = x << B;
        int rho = 1;
        while ((w & (1u << 31)) == 0 && rho <= 32 - B) {
            rho++;
            w <<= 1;
        }
        if (rho > regs[idx]) regs[idx] = rho;
    }

    double estimate() const {
        double am = alpha_m();
        double sum = 0;
        int zeros = 0;
        for (int i = 0; i < m; i++) {
            sum += ldexp(1.0, -regs[i]);
            if (regs[i] == 0) zeros++;
        }
        double E = am * m * m / sum;

        if (E <= 2.5 * m && zeros > 0) {
            E = m * log((double)m / zeros);
        } else {
            E = bias_correction(E);
        }

        if (E > (1.0 / 30.0) * pow(2.0, 32)) {
            E = -pow(2.0, 32) * log(1.0 - E / pow(2.0, 32));
        }

        return E;
    }

    int memory_bytes() const { return m; }
};


int main() {
    int N = 50000;
    int num_streams = 30;
    int B = 10;
    int num_parts = 20;
    ull base_seed = 42;

    vector<int> checkpoints;
    for (int p = 1; p <= num_parts; p++) {
        int t = (int)((long long)N * p / num_parts);
        if (checkpoints.empty() || checkpoints.back() != t)
            checkpoints.push_back(t);
    }
    int L = checkpoints.size();

    // статистики для стандартного HLL
    vector<long double> sum_est(L, 0), sum2_est(L, 0), sum_true(L, 0);
    // статистики для улучшенного HLL++
    vector<long double> sum_est_p(L, 0), sum2_est_p(L, 0);

    ofstream ts("out_timeseries.csv");
    ts << "stream,step,processed,true_unique,estimate,estimate_plus\n";

    for (int s = 0; s < num_streams; s++) {
        ull sseed = base_seed * 1000003 + s * 999983;
        RandomStreamGen gen(sseed);
        HyperLogLog hll(B, sseed ^ 0xABCDEF);
        HyperLogLogPlus hll_plus(B, sseed ^ 0xABCDEF);
        unordered_set<string> exact;
        exact.reserve(N * 2);

        int next_cp = 0;
        for (int i = 1; i <= N; i++) {
            string x = gen.next();
            exact.insert(x);
            hll.add(x);
            hll_plus.add(x);

            if (next_cp < L && i == checkpoints[next_cp]) {
                double tru = exact.size();
                double est = hll.estimate();
                double est_p = hll_plus.estimate();

                ts << s << "," << next_cp << "," << i
                   << "," << (size_t)tru << "," << est
                   << "," << est_p << "\n";

                sum_true[next_cp] += tru;
                sum_est[next_cp] += est;
                sum2_est[next_cp] += est * est;
                sum_est_p[next_cp] += est_p;
                sum2_est_p[next_cp] += est_p * est_p;

                next_cp++;
            }
        }
    }
    ts.close();

    ofstream st("out_stats.csv");
    st << "step,processed,mean_true,mean_est,std_est,mean_est_plus,std_est_plus\n";

    for (int j = 0; j < L; j++) {
        long double mt = sum_true[j] / num_streams;
        long double me = sum_est[j] / num_streams;
        long double var = sum2_est[j] / num_streams - me * me;
        if (var < 0) var = 0;
        long double sd = sqrt((double)var);

        long double me_p = sum_est_p[j] / num_streams;
        long double var_p = sum2_est_p[j] / num_streams - me_p * me_p;
        if (var_p < 0) var_p = 0;
        long double sd_p = sqrt((double)var_p);

        st << j << "," << checkpoints[j]
           << "," << (double)mt << "," << (double)me << "," << (double)sd
           << "," << (double)me_p << "," << (double)sd_p << "\n";
    }
    st.close();

    int m = 1 << B;
    cout << "B=" << B << " m=" << m << endl;
    cout << "theory 1.04/sqrt(m) = " << 1.04 / sqrt(m) << endl;
    cout << "theory 1.30/sqrt(m) = " << 1.30 / sqrt(m) << endl;
    cout << "memory (standard): " << m << " bytes" << endl;
    cout << "done: out_timeseries.csv, out_stats.csv" << endl;

    return 0;
}
