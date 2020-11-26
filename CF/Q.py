import math
k1, k2 = map(int, input().split())
n = int(input())
cnt1 = [0 for i in range(k1)]
cnt2 = [0 for i in range(k2)]
d = dict()
for i in range(n):
    x, y = map(int, input().split())
    x -= 1
    y -= 1
    cnt1[x] += 1
    if d.get((x, y)) is None:
        d[(x, y)] = 1
    else:
        d[(x, y)] += 1
ans = 0
v = d.items()
for t, q in v:
    ans += -q / n * math.log(q / cnt1[t[0]])
print(ans)