def get_gar(a, b):
    return 0 if a + b == 0 else (2 * a * b) / (a + b)


k = int(input())
c = []
for i in range(k):
    c.append(list(map(int, input().split())))
total = [0 for i in range(k)]
All = 0
for i in range(k):
    total[i] = sum(c[i])
    All += total[i]

TP = [0 for i in range(k)]
for i in range(k):
    TP[i] = c[i][i]

FP = [0 for i in range(k)]
for i in range(k):
    for j in range(k):
        if i != j:
            FP[i] += c[i][j]

FN = [0 for i in range(k)]
for i in range(k):
    for j in range(k):
        if i != j:
            FN[j] += c[i][j]

recall = [0.0 for i in range(k)]
precision = [0.0 for i in range(k)]
for i in range(k):
    recall[i] = 0 if FP[i] + TP[i] == 0 else TP[i] / (TP[i] + FP[i])
    precision[i] = 0 if FN[i] + TP[i] == 0 else TP[i] / (TP[i] + FN[i])

microAns = 0
for i in range(k):
    microAns += get_gar(precision[i], recall[i]) * total[i]
recallTotal = sum(TP) / All
mas = [0 if FN[i] + TP[i] == 0 else ((TP[i] * (FP[i] + TP[i])) / (FN[i] + TP[i])) for i in range(k)]
precisionTotal = sum(mas) / All

print(get_gar(recallTotal, precisionTotal))
print(microAns / All)