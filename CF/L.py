n = int(input())
x = []
y = []
for i in range(n):
    d = list(map(int, input().split()))
    x.append(d[0])
    y.append(d[1])
x_avg = sum(x) / len(x)
y_avg = sum(y) / len(y)
ch = 0
for i in range(n):
    ch += (x[i] - x_avg) * (y[i] - y_avg)
zn1 = 0
zn2 = 0
for i in range(n):
    zn1 += (x[i] - x_avg) ** 2
    zn2 += (y[i] - y_avg) ** 2
v = (zn1 * zn2) ** 0.5
if v == 0:
    print(0)
else:
    print(ch / v)