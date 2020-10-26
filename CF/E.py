import random


def getRandom(i, n):
    t = -1
    while True:
        t = random.randint(0, n - 1)
        if t != i:
            break
    return t

n = int(input())
K = [list() for i in range(n)]
Y = [0 for i in range(n)]
for i in range(n):
    a = list(map(int, input().split()))
    K[i], Y[i] = a[:-1], a[-1]
C = float(input())

def getE(ind):
    res = 0.0
    for i in range(n):
        res += alphas[i] * Y[i] * K[ind][i]
    return res + b - Y[ind]

def getLH(i, j, c):
    if Y[i] == Y[j]:
        L = max(0.0, alphas[i] + alphas[j] - c)
        H = min(alphas[i] + alphas[j], c)
    else:
        L = max(0.0, alphas[j] - alphas[i])
        H = min(c, c + alphas[j] - alphas[i])
    return L, H

alphas = [0.0 for i in range(n)]
passes = 0
max_passes = 5
b = 0.0
tol = 1e-7
xx = 0
while (xx < n * 5000):
    for i in range(n):
        xx += 1
        E_i = getE(i)
        # print("E_i=", E_i)
        if (E_i * Y[i] < -tol and alphas[i] < C) or (E_i * Y[i] > tol and alphas[i] > 0):
            j = getRandom(i, n)
            # print("j=", j)
            E_j = getE(j)
            alpha_i_old = alphas[i]
            alpha_j_old = alphas[j]
            L, H = getLH(i, j, C)
            # print(i, j, L, H)
            if abs(L - H) < tol:
                continue
            tetta = 2.0 * K[i][j] - K[i][i] - K[j][j]
            if tetta > -tol:
                continue
            new_alpha = alphas[j] - Y[j] * (E_i - E_j) / tetta
            alphas[j] = new_alpha
            if alphas[j] > H:
                alphas[j] = H
            elif alphas[j] < L:
                alphas[j] = L
            if abs(alphas[j] - alpha_j_old) < tol:
                continue
            alphas[i] = alphas[i] + Y[i] * Y[j] * (alpha_j_old - alphas[j])
            b1 = b - E_i - Y[i] * (alphas[i] - alpha_i_old) * K[i][i] - Y[j] * (alphas[j] - alpha_j_old) * K[i][j]
            b2 = b - E_j - Y[i] * (alphas[i] - alpha_i_old) * K[i][j] - Y[j] * (alphas[j] - alpha_j_old) * K[j][j]
            if 0 < alphas[i] < C:
                b = b1
            elif 0 < alphas[j] < C:
                b = b2
            else:
                b = (b1 + b2) / 2

for i in range(n):
    print(alphas[i])
print(b)
