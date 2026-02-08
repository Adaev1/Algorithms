CHARS = []
for c in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789":
    CHARS.append(c)

def ord_hash(c):
    return ord(c) - ord('a') + 1

def find_neutrals(p):
    result = []
    for c0 in CHARS:
        for c1 in CHARS:
            if ord_hash(c0) + ord_hash(c1) * p == 0:
                result.append(c0 + c1)
    return result

for p in range(1, 32):
    neutrals = find_neutrals(p)
    if neutrals:
        print(f"p = {p:2d}: {neutrals}")
    else:
        print(f"p = {p:2d}: нет нейтральных элементов")
