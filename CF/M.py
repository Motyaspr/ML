def get_rang(t):
    xs = []
    d = dict()
    for i in range(len(t)):
        if d.get(t[i]) is None:
            d[t[i]] = []
        d[t[i]].append(i)
    dt = {k: v for k, v in sorted(d.items(), key=lambda item: item[0])}
    r = 0
    ans = [0 for i in range(len(t))]
    v = dt.items()
    for it in v:
        for j in it[1]:
            ans[j] = r
        r += 1
    return ans


n = int(input())
x = []
y = []
x_ind = []
y_ind = []
for i in range(n):
    a, b = map(int, input().split())
    x.append(a)
    y.append(b)
ansx = get_rang(x)
ansy = get_rang(y)
v = 0
for i in range(n):
    v += (ansx[i] - ansy[i])**2
print(1 - (6 * v) / (n * (n - 1) * (n + 1)))