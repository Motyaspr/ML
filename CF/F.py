import math


def calc_p(alpha, cnt, total):
    return (cnt + alpha) / (total + alpha * 2)


def norm(scores, errors):
    mx = max(scores)
    ans = []
    for i in range(len(scores)):
        if scores[i]:
            ans.append(math.exp(scores[i] - mx) * errors[i])
        else:
            ans.append(0)
    return ans


K = int(input())
errors = list(map(int, input().split()))
aplha = int(input())
N = int(input())

classes = []
cnt_class = [0 for i in range(K)]
words_freq = [dict() for i in range(K)]
words = set()

for i in range(N):
    cur = input().split()
    type = int(cur[0]) - 1
    cnt_class[type] += 1
    L = int(cur[1])
    cur_words = set(cur[2:])
    for x in cur_words:
        words.add(x)
        if words_freq[type].get(x) is None:
            words_freq[type][x] = 0
        words_freq[type][x] += 1

for i in range(K):
    for x in words:
        if words_freq[i].get(x) is None:
            words_freq[i][x] = 0

words_probability = [dict() for i in range(K)]
for i in range(K):
    t = words_freq[i].items()
    for x in t:
        words_probability[i][x[0]] = calc_p(aplha, x[1], cnt_class[i])

M = int(input())
for i in range(M):
    cur = input().split()
    cur_words = set(cur[1:])
    scores = []
    for j in range(K):
        t = words_probability[j].items()
        res = 0
        for x in t:
            if x[0] in cur_words:
                res += math.log(x[1])
            else:
                res += math.log(1 - x[1])
        if cnt_class[j] == 0:
            scores.append(0)
        else:
            res += math.log(cnt_class[j] / N)
            scores.append(res)
    scores = norm(scores, errors)
    s = sum(scores)
    for x in scores:
        print(x / s, end=' ')
    print()
