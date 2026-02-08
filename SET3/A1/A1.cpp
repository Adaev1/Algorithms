#include <iostream>
#include <random>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Circle {
    double x, y, r;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Circle c[3];
    for (int i = 0; i < 3; ++i) {
        if (!(cin >> c[i].x >> c[i].y >> c[i].r)) return 0;
    }

    double xmin = c[0].x - c[0].r;
    double xmax = c[0].x + c[0].r;
    double ymin = c[0].y - c[0].r;
    double ymax = c[0].y + c[0].r;

    for (int i = 1; i < 3; ++i) {
        xmin = max(xmin, c[i].x - c[i].r);
        xmax = min(xmax, c[i].x + c[i].r);
        ymin = max(ymin, c[i].y - c[i].r);
        ymax = min(ymax, c[i].y + c[i].r);
    }

    if (xmin >= xmax || ymin >= ymax) {
        cout << fixed << setprecision(10) << 0.0;
        return 0;
    }

    double width = xmax - xmin;
    double height = ymax - ymin;
    double rect_area = width * height;

    const int N = 10000000;

    mt19937_64 rng(123456789);
    uniform_real_distribution<double> uni(0.0, 1.0);

    int inside = 0;

    for (int i = 0; i < N; ++i) {
        double rx = uni(rng);
        double ry = uni(rng);
        double x = xmin + width * rx;
        double y = ymin + height * ry;

        bool ok = true;
        for (int j = 0; j < 3; ++j) {
            double dx = x - c[j].x;
            double dy = y - c[j].y;
            if (dx * dx + dy * dy > c[j].r * c[j].r) {
                ok = false;
                break;
            }
        }
        if (ok) ++inside;
    }

    double estimate = rect_area * (static_cast<double>(inside) / N);

    cout << fixed << setprecision(15) << estimate;
    return 0;
}