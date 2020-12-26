#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <assert.h>

const int MAXN = 4003;

int classes[MAXN];

int n, m, k, H;

std::vector<int> pr[MAXN];

std::vector<std::pair<int, std::string>> a;

int total_cnt = 0;

int max_ind = 1;

struct Node {
    int num;
    int h;
    bool is_list;
    Node *l;
    Node *r;
    std::vector<size_t> items;

    std::pair<size_t, double> ind;

    Node() = default;

    Node(int new_h, std::vector<size_t> new_items, int new_num) {
        h = new_h;
        items = std::move(new_items);
        l = nullptr;
        r = nullptr;
        is_list = false;
        num = new_num;
    }

};

bool is_one_class(Node *t) {
    std::set<int> s;
    for (unsigned long item : t->items)
        s.insert(classes[item]);
    return s.size() == 1;
}

double calc(std::vector<size_t> left, std::vector<size_t> right) {

}

std::pair<double, double> get_best_gini(std::vector<std::pair<int, size_t>> &values) {
    int r_cnt = 0;
    int l_cnt = values.size();
    int total = values.size();
    std::vector<int> ls(k + 1, 0), rs(k + 1, 0);
    for (auto &value : values)
        ls[classes[value.second]]++;
    double max_score = 0;
    double sum_l = 0;
    double sum_r = 0;
    for (size_t i = 1; i <= k; i++)
        sum_l += ls[i] * ls[i];
    max_score = 10000.0;
    double best_num = values[0].first;
    size_t ind = 0;
    while (ind < values.size()) {
        int ind_st = ind;
        int ind_en = ind;
        while (ind_en < values.size() && values[ind_en].first == values[ind_st].first)
            ind_en++;
        for (size_t i = ind_st; i < ind_en; i++) {
            int clas = classes[values[i].second];
            sum_l -= ls[clas] * ls[clas];
            ls[clas]--;
            sum_l += ls[clas] * ls[clas];
            sum_r -= rs[clas] * rs[clas];
            rs[clas]++;
            sum_r += rs[clas] * rs[clas];
            l_cnt--;
            r_cnt++;
        }
        double score = 0;
        if (l_cnt == 0) {
            score = 1.0 + (double) (1 - sum_r) / (r_cnt * total);
        } else
            score = (0.1 - sum_l) / (l_cnt * total) + (0.1 - sum_r) / (r_cnt * total);
        //std::cout << values[ind_st].first + 0.5 << ' ' << score << "\n";
        if (score < max_score) {
            max_score = score;
            if (ind_en == values.size())
                best_num = (double) values[ind_st].first + 0.5;
            else
                best_num = (double) (values[ind_en].first + values[ind_st].first) / 2.0;

        }
        ind = ind_en;
    }
    //std::cout << "MAX_score" << max_score << "\n";
    return {max_score, best_num};
}

void split_node(Node *t) {
    if (t->h == H) {
        t->is_list = true;
        return;
    }
    if (is_one_class(t)) {
        t->is_list = true;
        return;
    }
    double best_score = 10000;
    std::pair<size_t, double> ind;
    for (size_t i = 0; i < m; i++) {
        std::vector<std::pair<int, size_t>> values;
        values.reserve(t->items.size());
        for (size_t j = 0; j < t->items.size(); j++)
            values.emplace_back(pr[t->items[j]][i], t->items[j]);
        sort(values.begin(), values.end());
        auto x = get_best_gini(values);
        if (x.first < best_score) {
            best_score = x.first;
            ind = {i, x.second};
        }

    }
    std::vector<size_t> l, r;
    for (unsigned long item : t->items)
        if (pr[item][ind.first] < ind.second) {
            l.push_back(item);
        } else {
            r.push_back(item);
        }
    if (l.empty() || r.empty()) {
        assert(1 == 0);
        t->is_list = true;
        return;
    }
    t->ind = ind;
    max_ind++;
    t->l = new Node(t->h + 1, l, max_ind);
    max_ind++;
    t->r = new Node(t->h + 1, r, max_ind);
}

void dfs(Node *t) {
    split_node(t);
    total_cnt++;
    if (t->is_list)
        return;
    dfs(t->l);
    dfs(t->r);
}

void print(Node *t) {
    if (t->is_list) {
        std::string answer;
        std::vector<int> max_cnt = std::vector<int>(k + 1, 0);
        for (unsigned long item : t->items)
            max_cnt[classes[item]]++;
        int maxi = 0;
        int ans = 0;
        for (size_t i = 1; i <= k; i++)
            if (max_cnt[i] >= maxi) {
                maxi = max_cnt[i];
                ans = i;
            }
        answer = "C " + std::to_string(ans);
        a.emplace_back(t->num, answer);
        return;
    }
    std::string answer = "Q ";
    answer += std::to_string(t->ind.first + 1);
    answer += " ";
    answer += std::to_string(t->ind.second);
    answer += " ";
    answer += std::to_string(t->l->num);
    answer += " ";
    answer += std::to_string(t->r->num);
    a.emplace_back(t->num, answer);
    print(t->l);
    print(t->r);
}

int main() {
    std::cin >> m >> k >> H >> n;
    std::vector<size_t> indexes;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            int x;
            std::cin >> x;
            pr[i].push_back(x);
        }
        int x;
        std::cin >> x;
        classes[i] = x;
        indexes.push_back(i);
    }
    Node *t = new Node(0, indexes, max_ind);
    dfs(t);
    std::cout << total_cnt << "\n";
    print(t);
    std::sort(a.begin(), a.end());
    for (auto &i : a)
        std::cout << i.second << "\n";
    return 0;
}

