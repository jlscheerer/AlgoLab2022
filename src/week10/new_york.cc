#include <bits/stdc++.h>

using namespace std;

class window {
public:
  int size() { return order.size() - offset; }
  int min() {
    while (counts[min_pq.top()] == 0) {
      min_pq.pop();
    }
    return min_pq.top();
  }
  int max() {
    while (counts[max_pq.top()] == 0) {
      max_pq.pop();
    }
    return max_pq.top();
  }
  void push_back(int index, int value) {
    min_pq.push(value);
    max_pq.push(value);
    ++counts[value];
    order.push_back({index, value});
  }
  void push_front() {
    --offset;
    pair<int, int> value = order[offset];
    ++counts[value.second];
    min_pq.push(value.second);
    max_pq.push(value.second);
  }
  int pop_back() {
    pair<int, int> value = order.back();
    order.pop_back();
    --counts[value.second];
    return value.first;
  }
  pair<int, int> pop_front() {
    pair<int, int> value = order[offset];
    // order.pop_front();
    ++offset;
    --counts[value.second];
    return value;
  }

private:
  priority_queue<int, vector<int>, greater<>> min_pq;
  priority_queue<int, vector<int>, less<>> max_pq;
  unordered_map<int, int> counts;
  deque<pair<int, int>> order;

  int offset = 0;
};

void dfs(const int n, const int k, const int m,
         unordered_map<int, vector<int>> &adj, vector<int> &h, int node,
         window &w, vector<bool> &ok) {
  w.push_back(node, h[node]);
  int count = 0;
  while (w.size() && (h[node] - w.min() > k || w.max() - h[node] > k)) {
    w.pop_front();
    ++count;
  }
  while (w.size() >= m) {
    int end = w.pop_front().first;
    ok[end] = true;
    ++count;
  }
  for (int child : adj[node]) {
    dfs(n, k, m, adj, h, child, w, ok);
  }
  for (int i = 0; i < count; ++i) {
    w.push_front();
  }
  w.pop_back();
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> h(n);
    for (int i = 0; i < n; ++i) {
      cin >> h[i];
    }
    unordered_map<int, vector<int>> adj;
    for (int i = 0; i < n - 1; ++i) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
    }
    window w;
    vector<bool> ok(n);
    dfs(n, k, m, adj, h, 0, w, ok);
    bool printed = false;
    for (int i = 0; i < n; ++i) {
      if (!ok[i])
        continue;
      if (printed)
        cout << ' ';
      cout << i;
      printed = true;
    }
    if (printed)
      cout << '\n';
    else
      cout << "Abort mission\n";
  }
}