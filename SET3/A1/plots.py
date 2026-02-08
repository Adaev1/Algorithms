import matplotlib.pyplot as plt
from math import acos, asin

N = []
S_wide = []
S_narrow = []
rel_err_wide = []
rel_err_narrow = []

with open("data.txt", encoding="utf-8") as f:
    header = f.readline()
    for line in f:
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split()
        if len(parts) < 5:
            continue
        n, sw, sn, rw, rn = parts
        N.append(int(n))
        S_wide.append(float(sw))
        S_narrow.append(float(sn))
        rel_err_wide.append(float(rw))
        rel_err_narrow.append(float(rn))

PI = acos(-1.0)
S_exact = 0.25 * PI + 1.25 * asin(0.8) - 1.0

plt.figure(figsize=(10, 5))
plt.plot(N, S_wide, label="Широкий прямоугольник")
plt.plot(N, S_narrow, label="Узкий прямоугольник")
plt.axhline(S_exact, linestyle="--", label="Точная площадь")
plt.xlabel("Число точек N")
plt.ylabel("Оценка площади S̃")
plt.title("Оценка площади пересечения трёх окружностей")
plt.legend()
plt.grid(True)

plt.figure(figsize=(10, 5))
plt.plot(N, rel_err_wide, label="Широкий прямоугольник")
plt.plot(N, rel_err_narrow, label="Узкий прямоугольник")
plt.xlabel("Число точек N")
plt.ylabel("Относительная ошибка (S̃ - S_exact)/S_exact")
plt.title("Относительная ошибка метода Монте-Карло")
plt.legend()
plt.grid(True)

plt.show()
