k = int(input())
n = int(input())
classes = [[] for i in range(k + 1)]


def get_ans(t):
    t.sort()
    sm = sum(t)
    msm = 0
    ans = 0
    for i in range(len(t)):
        sm -= t[i]
        msm += t[i]
        cnt1 = i + 1
        cnt2 = len(t) - i - 1
        ans += (t[i] * cnt1 - msm) + (sm - cnt2 * t[i])
    return ans

arr = []
for i in range(n):
    x, y = map(int, input().split())
    classes[y].append(x)
    arr.append(x)

in_ans = 0
for i in range(k + 1):
    if len(classes[i]) > 1:
        in_ans += get_ans(classes[i])
print(in_ans)
sum_ans = get_ans(arr)
print(sum_ans - in_ans)