import pandas as pd
import matplotlib.pyplot as plt

ts = pd.read_csv("out_timeseries.csv")
st = pd.read_csv("out_stats.csv")

# График 1: сравнение N_t и F_0^t
stream0 = ts[ts["stream"] == 0]

plt.figure(figsize=(10, 6))
plt.plot(stream0["processed"], stream0["true_unique"], "b-o", label="$F_0^t$ (точное)", markersize=4)
plt.plot(stream0["processed"], stream0["estimate"], "r--s", label="$N_t$ (HyperLogLog)", markersize=4)
plt.plot(stream0["processed"], stream0["estimate_plus"], "g:^", label="$N_t$ (HyperLogLog++)", markersize=4)
plt.xlabel("Обработано элементов")
plt.ylabel("Количество уникальных элементов")
plt.title("Сравнение оценки HyperLogLog с точным значением")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("graph1.png", dpi=150)
plt.close()

# График 2: E(N_t) ± sigma
plt.figure(figsize=(10, 6))
plt.plot(st["processed"], st["mean_true"], "b-o", label="$F_0^t$ (среднее точное)", markersize=4)
plt.plot(st["processed"], st["mean_est"], "r-s", label="$E(N_t)$ (HyperLogLog)", markersize=4)
plt.fill_between(st["processed"],
                 st["mean_est"] - st["std_est"],
                 st["mean_est"] + st["std_est"],
                 color="red", alpha=0.15, label="$E(N_t) \\pm \\sigma$")

plt.plot(st["processed"], st["mean_est_plus"], "g-^", label="$E(N_t)$ (HyperLogLog++)", markersize=4)
plt.fill_between(st["processed"],
                 st["mean_est_plus"] - st["std_est_plus"],
                 st["mean_est_plus"] + st["std_est_plus"],
                 color="green", alpha=0.15, label="$E(N_t) \\pm \\sigma$ (++)")

plt.xlabel("Обработано элементов")
plt.ylabel("Количество уникальных элементов")
plt.title("Статистики оценки HyperLogLog (30 потоков)")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("graph2.png", dpi=150)
plt.close()

print("done")
