#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

struct Node {
    int r, c;
    vector<vector<double>> value;
    vector<vector<double>> diff;

    vector<size_t> indexes;

    string type;

    Node(string _type, vector<size_t> ind) {
        type = move(_type);
        r = 0;
        c = 0;
        indexes = move(ind);
    }

    Node(int _r, int _c, string _type, vector<size_t> index) {
        r = _r;
        c = _c;
        indexes = move(index);
        type = std::move(_type);
        value.resize(r);
        diff.resize(r);
        for (size_t i = 0; i < r; i++) {
            value[i].resize(c);
            diff[i].resize(c);
        }
//        diff.clear();
    }

    Node(vector<vector<double>> &other) {
        r = other.size();
        c = other.back().size();
        value.resize(r);
        diff.resize(r);
        for (size_t i = 0; i < r; i++)
            for (size_t j = 0; j < c; j++) {
                value[i].push_back(other[i][j]);
                diff[i].push_back(0.);
            }
        type = "var";
    }

    void read_diff(vector<vector<double>> &other) {
        for (size_t i = 0; i < other.size(); i++)
            for (size_t j = 0; j < other[i].size(); j++)
                diff[i][j] = other[i][j];
    }

    void print() {
        for (size_t i = 0; i < r; i++) {
            for (size_t j = 0; j < c; j++)
                std::cout << fixed << value[i][j] << ' ';
            cout << "\n";
        }
    }

    void print_diff() {
        for (size_t i = 0; i < r; i++) {
            for (size_t j = 0; j < c; j++)
                std::cout << fixed << diff[i][j] << ' ';
            cout << "\n";
        }
    }
};

vector<Node> ans;

Node tanh(vector<size_t> indexes) {
    Node t = ans[indexes.front()];
    Node new_t = Node(t.r, t.c, "tnh", indexes);
    for (size_t i = 0; i < t.r; i++) {
        for (size_t j = 0; j < t.c; j++) {
            new_t.value[i][j] = tanh(t.value[i][j]);
        }
    }
    return new_t;
}

Node rlu(vector<size_t> indexes, int alph) {
    Node t = ans[indexes.front()];
    Node new_t = Node(t.r, t.c, "rlu", move(indexes));
    for (size_t i = 0; i < t.r; i++) {
        for (size_t j = 0; j < t.c; j++) {
            new_t.value[i][j] = t.value[i][j] >= 0 ? t.value[i][j] : t.value[i][j] / alph;
        }
    }
    return new_t;
}

Node mul(vector<size_t> indexes) {
    auto a = ans[indexes.front()];
    auto b = ans[indexes[1]];
    Node new_t = Node(a.r, b.c, "mul", indexes);
    for (size_t i = 0; i < new_t.r; i++)
        for (size_t j = 0; j < new_t.c; j++)
            for (size_t k = 0; k < a.c; k++)
                new_t.value[i][j] += a.value[i][k] * b.value[k][j];
    return new_t;
}

Node sum(vector<size_t> indexes) {
    Node new_t = Node(ans[indexes.front()].r, ans[indexes.front()].c, "sum", indexes);
    for (auto &ind : indexes)
        for (size_t i = 0; i < new_t.r; i++)
            for (size_t j = 0; j < new_t.c; j++)
                new_t.value[i][j] += ans[ind].value[i][j];
    return new_t;
}

Node had(vector<size_t> a) {
    Node new_t = Node(ans[a.front()].r, ans[a.front()].c, "had", a);
    for (size_t i = 0; i < new_t.r; i++)
        for (size_t j = 0; j < new_t.c; j++)
            new_t.value[i][j] = 1;
    for (auto &ind : a)
        for (size_t i = 0; i < new_t.r; i++)
            for (size_t j = 0; j < new_t.c; j++)
                new_t.value[i][j] *= ans[ind].value[i][j];
    return new_t;
}

void d_tanh(Node &t) {
    for (size_t i = 0; i < t.r; i++) {
        for (size_t j = 0; j < t.c; j++) {
            ans[t.indexes.front()].diff[i][j] += t.diff[i][j] * (1 - t.value[i][j] * t.value[i][j]);
        }
    }
}

void d_rlu(Node &t, int alphas) {
    for (size_t i = 0; i < t.r; i++)
        for (size_t j = 0; j < t.c; j++) {
            ans[t.indexes.front()].diff[i][j] += (ans[t.indexes.front()].value[i][j] >= 0) ? t.diff[i][j] :
                                                 t.diff[i][j] / alphas;
        }
}

void d_had(Node &t) {
    for (size_t i = 0; i < t.r; i++)
        for (size_t j = 0; j < t.c; j++) {
            for (size_t cur = 0; cur < t.indexes.size(); cur++) {
                double cur_mul = 1;
                for (size_t nxt = 0; nxt < t.indexes.size(); nxt++) {
                    if (nxt != cur)
                        cur_mul *= ans[t.indexes[nxt]].value[i][j];
                }
                ans[t.indexes[cur]].diff[i][j] += cur_mul * t.diff[i][j];
            }
        }
}

void d_mul(Node &t) {
    for (size_t i = 0; i < ans[t.indexes.front()].r; i++)
        for (size_t j = 0; j < ans[t.indexes.front()].c; j++) {
            for (size_t q = 0; q < t.c; q++)
                ans[t.indexes.front()].diff[i][j] += t.diff[i][q] * ans[t.indexes.back()].value[j][q];
        }
    for (size_t i = 0; i < ans[t.indexes.back()].r; i++)
        for (size_t j = 0; j < ans[t.indexes.back()].c; j++) {
            for (size_t q = 0; q < t.r; q++)
                ans[t.indexes.back()].diff[i][j] += t.diff[q][j] * ans[t.indexes.front()].value[q][i];
        }
}

void d_sum(Node &t) {
    for (size_t ind = 0; ind < t.indexes.size(); ind++)
        for (size_t i = 0; i < t.r; i++)
            for (size_t j = 0; j < t.c; j++)
                ans[t.indexes[ind]].diff[i][j] += t.diff[i][j];
}

int n, m, k;
vector<pair<string, vector<size_t>>> zap;

int main() {
    cout.precision(5);
    cin >> n >> m >> k;
    vector<int> alphas;
    alphas.resize(n, 0);
    for (size_t i = 0; i < n; i++) {
        vector<size_t> t;
        string s;
        size_t x, y;
        cin >> s;
        if (i < m) {
            cin >> x >> y;
            t.push_back(x);
            t.push_back(y);
            ans.emplace_back(x, y, "var", vector<size_t>());
        } else {
            if (s == "sum" || s == "had") {
                cin >> x;
                for (size_t j = 0; j < x; j++) {
                    cin >> y;
                    t.push_back(y - 1);
                }
            } else {
                if (s == "mul") {
                    cin >> x >> y;
                    t.push_back(x - 1);
                    t.push_back(y - 1);
                } else {
                    if (s == "rlu") {
                        int kek;
                        cin >> kek;
                        alphas[i] = kek;
                    }
                    cin >> x;
                    t.push_back(x - 1);
                }
            }
        }
        zap.emplace_back(s, t);
    }
    int cur = 0;
    for (int ind = 0; ind < m; ind++) {
        for (size_t i = 0; i < ans[ind].r; i++)
            for (size_t j = 0; j < ans[ind].c; j++) {
                int x;
                cin >> x;
                ans[ind].value[i][j] = x;
            }
    }

    for (size_t i = m; i < n; i++) {
        if (zap[i].first == "tnh")
            ans.push_back(tanh(zap[i].second));
        if (zap[i].first == "rlu")
            ans.push_back(rlu(zap[i].second, alphas[i]));
        if (zap[i].first == "mul")
            ans.push_back(mul(zap[i].second));
        if (zap[i].first == "sum")
            ans.push_back(sum(zap[i].second));
        if (zap[i].first == "had")
            ans.push_back(had(zap[i].second));
    }
    for (size_t i = n - k; i < n; i++) {
        ans[i].print();
    }
    for (size_t ind = n - k; ind < n; ind++) {
        for (size_t i = 0; i < ans[ind].r; i++)
            for (size_t j = 0; j < ans[ind].c; j++) {
                int x;
                cin >> x;
                ans[ind].diff[i][j] = x;
            }
    }
    for (int i = n - 1; i >= 0; i--) {
        if (ans[i].type == "tnh")
            d_tanh(ans[i]);
        else if (ans[i].type == "rlu")
            d_rlu(ans[i], alphas[i]);
        else if (ans[i].type == "mul")
            d_mul(ans[i]);
        else if (ans[i].type == "sum")
            d_sum(ans[i]);
        else if (ans[i].type == "had")
            d_had(ans[i]);
    }
    for (size_t i = 0; i < m; i++) {
        ans[i].print_diff();
    }

}