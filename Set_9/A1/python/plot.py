import json
import os
import matplotlib.pyplot as plt

ALGO_ORDER = [
    "Std QuickSort",
    "Std MergeSort",
    "String QuickSort",
    "String MergeSort",
    "MSD Radix Sort",
    "MSD Radix + QuickSort",
]

ALGO_COLORS = {
    "Std QuickSort":         "tab:red",
    "Std MergeSort":         "tab:orange",
    "String QuickSort":      "tab:blue",
    "String MergeSort":      "tab:cyan",
    "MSD Radix Sort":        "tab:purple",
    "MSD Radix + QuickSort": "tab:green",
}

KIND_TITLES = {
    "random":         "Случайные строки",
    "almost_sorted":  "Почти отсортированные строки",
    "reverse_sorted": "Обратно отсортированные строки",
}


def plot_one(kind, data, out_path):
    ns = data["_ns"]
    fig, axes = plt.subplots(1, 2, figsize=(13, 5))
    fig.suptitle(KIND_TITLES[kind])

    for name in ALGO_ORDER:
        series = data[name]
        axes[0].plot(ns, series["time_ms"], marker=".",
                     label=name, color=ALGO_COLORS[name])
        axes[1].plot(ns, series["comparisons"], marker=".",
                     label=name, color=ALGO_COLORS[name])

    axes[0].set_xlabel("Количество строк (N)")
    axes[0].set_ylabel("Время (мс)")
    axes[0].grid(True, alpha=0.3)
    axes[0].legend(fontsize=8)

    axes[1].set_xlabel("Количество строк (N)")
    axes[1].set_ylabel("Сравнения символов")
    axes[1].grid(True, alpha=0.3)
    axes[1].legend(fontsize=8)

    fig.tight_layout()
    fig.savefig(out_path, dpi=130)
    plt.close(fig)
    print(f"saved {out_path}")


def main():
    here = os.path.dirname(os.path.abspath(__file__))
    results_dir = os.path.normpath(os.path.join(here, "..", "results"))
    with open(os.path.join(results_dir, "measurements.json")) as f:
        all_data = json.load(f)
    for kind in ("random", "almost_sorted", "reverse_sorted"):
        out_path = os.path.join(results_dir, f"plot_{kind}.png")
        plot_one(kind, all_data[kind], out_path)


if __name__ == "__main__":
    main()
