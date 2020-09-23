import math


def manhattan(a, b):
    d = 0
    for i in range(len(a)):
        d += abs(a[i] - b[i])
    return d


def euclide(a, b):
    d = 0
    for i in range(len(a)):
        d += (a[i] - b[i]) ** 2
    return d ** 0.5


def chebyshev(a, b):
    d = -1
    for i in range(len(a)):
        d = max(d, abs(a[i] - b[i]))
    return d


def uniform(v):
    if abs(v) < 1:
        return 0.5
    return 0


def triangular(v):
    if abs(v) < 1:
        return 1 - abs(v)
    return 0


def epanechnikov(v):
    if abs(v) < 1:
        return 3 / 4 * (1 - v ** 2)
    return 0


def quartic(v):
    if abs(v) < 1:
        return 15 / 16 * (1 - v ** 2) ** 2
    return 0


def triweight(v):
    if abs(v) < 1:
        return 35 / 32 * (1 - v ** 2) ** 3
    return 0


def tricube(v):
    if abs(v) < 1:
        return ((1 - abs(v) ** 3) ** 3) * 70 / 81
    return 0


def gaussian(v):
    return (math.e ** (-0.5 * (v ** 2))) / math.sqrt(2 * math.pi)


def cosine(v):
    if abs(v) < 1:
        return (math.pi / 4) * math.cos((math.pi / 2) * v)
    return 0


def logistic(v):
    return 1 / (math.exp(v) + 2 + math.exp(-v))


def sigmoid(v):
    return (2 / math.pi) / (math.exp(v) + math.exp(-v))


functions = {
    "uniform": uniform,
    "triangular": triangular,
    "quartic": quartic,
    "epanechnikov": epanechnikov,
    "triweight": triweight,
    "tricube": tricube,
    "gaussian": gaussian,
    "cosine": cosine,
    "logistic": logistic,
    "sigmoid": sigmoid
}

distances = {
    "manhattan": manhattan,
    "euclidean": euclide,
    "chebyshev": chebyshev
}


def getPrediction(dType, kType, wType, h, params, x, query):
    distance_function = distances[dType]
    kernel_function = functions[kType]
    window = wType

    vectorDistances = [(distance_function(params[i], query), x[i]) for i in range(len(x))]

    vectorDistances = sorted(vectorDistances)

    divisor = h if window == "fixed" else vectorDistances[h][0]

    sum_x = 0
    sum_y = 0
    if divisor == 0:
        for i in vectorDistances:
            if i[0] == 0:
                sum_x += 1
                sum_y += i[1]
    else:
        for cur in vectorDistances:
            v = cur[0] / divisor
            k = kernel_function(v)
            sum_x += k
            sum_y += cur[1] * k

    if sum_x == 0:
        return sum(x) / len(x)
    else:
        return sum_y / sum_x


n, m = map(int, input().split())

dataset = [[] for i in range(n)]
x = [0 for i in range(n)]

for i in range(n):
    arr = list(map(int, input().split()))
    dataset[i], x[i] = arr[:-1], arr[-1]

query = list(map(int, input().split()))
dType = input()
kType = input()
wType = input()
h = int(input())

print(getPrediction(dType, kType, wType, h, dataset, x, query))
