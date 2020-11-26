k1, k2 = map(int, input().split())
n = int(input())
cnt1 = [0 for i in range(k1)]
cnt2 = [0 for i in range(k2)]
d = dict()
for i in range(n):
    x, y = map(int, input().split())
    x -= 1
    y -= 1
    cnt1[x] += 1 / n
    cnt2[y] += 1 / n
    if d.get((x, y)) is None:
        d[(x, y)] = 1
    else:
        d[(x, y)] += 1
v = d.items()
ans = n
for t, q in v:
    res1 = n * cnt1[t[0]] * cnt2[t[1]]
    ans = ans - res1 + ((q - res1) * (q -res1) / res1)
print(ans)