import random


m = int(input())

a = list()
for i in range(2 ** m):
    x = int(input())
    if x == 0:
        continue
    a.append(i)
if len(a) == 0:
    print("1\n1")
    for i in range(m + 1):
        print("-1.0", end = ' ')
    exit(0)
K = min(len(a), 512)
print(2)
print(K, 1)
for i in range(K):
    x = a[i]
    b = 0.5
    s = ""
    for j in range(m):
        if x & (1 << j):
            s += "1.0 "
            b -= 1
        else:
            s += "-10.0 "
    s += str(b)
    print(s)
for i in range(K + 1):
    if i == K:
        print(-0.5, end = ' ')
    else:
        print(1.0, end = ' ')
