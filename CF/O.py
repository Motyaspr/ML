k = int(input())
n = int(input())
cnt = [0 for i in range(k)]
e = [0 for i in range(k)]
sum1 = 0
for i in range(n):
    x, y = map(int, input().split())
    x -= 1
    cnt[x] += 1
    e[x] += y / n
    sum1 += y * y / n
sum2 = 0
for i in range(k):
    if cnt[i]:
        sum2 += e[i] * e[i] / (cnt[i] / n)
print(sum1 - sum2)
