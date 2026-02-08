import matplotlib.pyplot as plt

filename = "data_a2.txt"

thresholds = []
data = {}

with open(filename, encoding="utf-8") as f:
    for line in f:
        s = line.strip()
        if not s:
            continue
        if s.lstrip().startswith("#"):
            if not thresholds:
                cols = s.lstrip("#").split()
                for name in cols[3:]:
                    if name.startswith("hybrid_"):
                        thresholds.append(int(name.split("_")[1]))
            continue
        parts = s.split()
        if len(parts) < 3:
            continue
        kind = int(parts[0])
        n = int(parts[1])
        t_merge = float(parts[2])
        t_vals = [float(x) for x in parts[3:3 + len(thresholds)]]
        if kind not in data:
            data[kind] = {
                "N": [],
                "merge": [],
                "hybrid": {t: [] for t in thresholds},
            }
        d = data[kind]
        d["N"].append(n)
        d["merge"].append(t_merge)
        for t, v in zip(thresholds, t_vals):
            d["hybrid"][t].append(v)

labels = {
    0: "случайные массивы",
    1: "отсортированные по невозрастанию",
    2: "«почти» отсортированные массивы",
}

for kind in sorted(data.keys()):
    d = data[kind]
    plt.figure(figsize=(10, 6))
    plt.plot(d["N"], d["merge"], label="MERGE SORT")
    for t in thresholds:
        plt.plot(d["N"], d["hybrid"][t], label=f"MERGE+INSERTION (k={t})")
    plt.xlabel("N")
    plt.ylabel("время (мкс)")
    plt.title(labels.get(kind, str(kind)))
    plt.legend()
    plt.grid(True)

plt.show()
