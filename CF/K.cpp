#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <assert.h>
#include <cmath>

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

    void read_diff() {
        for (size_t i = 0; i < r; i++)
            for (size_t j = 0; j < c; j++) {
                int x;
                cin >> x;
                diff[i][j] = x;
            }
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

Node sigma(vector<size_t> indexes) {
    Node t = ans[indexes[0]];
    Node new_t = Node(t.r, t.c, "sigma", indexes);
    for (size_t i = 0; i < t.r; i++)
        for (size_t j = 0; j < t.c; j++)
            new_t.value[i][j] = 1.0 / (1 + exp(-t.value[i][j]));
    return new_t;
}

Node tanh(vector<size_t> indexes) {
    Node t = ans[indexes[0]];
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

void d_sigm(Node &t) {
    for (size_t i = 0; i < t.r; i++) {
        for (size_t j = 0; j < t.c; j++) {
            ans[t.indexes.front()].diff[i][j] += t.diff[i][j] * (1 - t.value[i][j]) * t.value[i][j];
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

void read_mt(size_t &x) {
    x = ans.size();
    vector<vector<double>> q;
    q.resize(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            int v;
            cin >> v;
            q[i].push_back(v);
        }
    ans.emplace_back(Node(q));
}

void read_vec(size_t &x) {
    x = ans.size();
    vector<vector<double>> q;
    q.resize(n);
    for (auto & i : q) {
        int t;
        cin >> t;
        i.push_back(t);
    }
    ans.emplace_back(Node(q));
}

size_t calc(size_t x, size_t w, size_t u, size_t b, size_t last_h, bool f) {
    vector<size_t> indexes;
    indexes.push_back(w);
    indexes.push_back(x);
    ans.push_back(mul(indexes));
    indexes.clear();
    indexes.push_back(u);
    indexes.push_back(last_h);
    ans.push_back(mul(indexes));
    indexes.clear();
    indexes.push_back(ans.size() - 2);
    indexes.push_back(ans.size() - 1);
    indexes.push_back(b);
    ans.push_back(sum(indexes));
    indexes.clear();
    indexes.push_back(ans.size() - 1);
    if (f)
        ans.push_back(sigma(indexes));
    return ans.size() - 1;
}



int main() {
    cout.precision(15);
    cin >> n;
    size_t w_f, u_f, b_f, w_i, u_i, b_i, w_o, b_o, u_o, w_c, b_c, u_c;
    read_mt(w_f);
    read_mt(u_f);
    read_vec(b_f);
    read_mt(w_i);
    read_mt(u_i);
    read_vec(b_i);
    read_mt(w_o);
    read_mt(u_o);
    read_vec(b_o);
    read_mt(w_c);
    read_mt(u_c);
    read_vec(b_c);
    cin >> m;
    size_t h, c;
    read_vec(h);
    read_vec(c);
    swap(c, h);
    size_t last_h = h;
    size_t last_c = c;
    swap(ans.back(), ans[ans.size() - 2]);
    vector<size_t> xs;
    xs.resize(m, 0);
    for (size_t i = 0; i < m; i++)
        read_vec(xs[i]);
    vector<size_t> os;
    for (size_t i = 0; i < m; i++) {
        size_t x = xs[i];
        vector<size_t> indexes;
        //f
        size_t last_f = calc(x, w_f, u_f, b_f, last_h, true);
        size_t last_i = calc(x, w_i, u_i, b_i, last_h, true);
        os.push_back(calc(x, w_o, u_o, b_o, last_h, true));
        size_t last_another_c = calc(x, w_c, u_c, b_c, last_h, false);
        indexes.push_back(last_f);
        indexes.push_back(last_c);
        ans.push_back(had(indexes));
        size_t fir_ind = ans.size() - 1;
        indexes.clear();
        indexes.push_back(last_another_c);
        ans.push_back(tanh(indexes));

        indexes.clear();
        indexes.push_back(last_i);
        indexes.push_back(ans.size() - 1);
        ans.push_back(had(indexes));
        indexes.clear();
        indexes.push_back(ans.size() - 1);
        indexes.push_back(fir_ind);
        ans.push_back(sum(indexes));
        last_c = ans.size() - 1;
        indexes.clear();
        indexes.push_back(last_c);
        indexes.push_back(os.back());
        ans.push_back(had(indexes));
        last_h = ans.size() - 1;
    }
    for (unsigned long o : os) {
        ans[o].print();
    }
    ans[last_h].print();
    ans[last_c].print();
    ans[last_h].read_diff();
    ans[last_c].read_diff();
    for (int i = os.size() - 1; i >= 0; i--)
        ans[os[i]].read_diff();
    for (int i = ans.size() - 1; i >= 0; i--) {
        if (ans[i].type == "tnh")
            d_tanh(ans[i]);
        else if (ans[i].type == "mul")
            d_mul(ans[i]);
        else if (ans[i].type == "sum")
            d_sum(ans[i]);
        else if (ans[i].type == "had")
            d_had(ans[i]);
        else if (ans[i].type == "sigma")
            d_sigm(ans[i]);
    }
    for (int i = xs.size() - 1; i >= 0; i--)
        ans[xs[i]].print_diff();
    ans[h].print_diff();
    ans[c].print_diff();
    ans[w_f].print_diff();
    ans[u_f].print_diff();
    ans[b_f].print_diff();
    ans[w_i].print_diff();
    ans[u_i].print_diff();
    ans[b_i].print_diff();
    ans[w_o].print_diff();
    ans[u_o].print_diff();
    ans[b_o].print_diff();
    ans[w_c].print_diff();
    ans[u_c].print_diff();
    ans[b_c].print_diff();
}