#include <bits/stdc++.h>

using namespace std;

// [no_repair, indirect_repair, repair]
tuple<int, int, int> recsolve(int n, vector<vector<int>> &adj, vector<int> &c,
                              int u) {
  if (adj[u].size() == 0) { // leaf
    return {0, c[u], c[u]};
  }
  int sum_no_repair = 0, sum_indirect = 0,
      candidate = INT_MAX / 8; // = repair_i - indirect_i
  for (int v : adj[u]) {
    int no_repair, indirect_repair, repair;
    tie(no_repair, indirect_repair, repair) = recsolve(n, adj, c, v);
    sum_no_repair += no_repair;
    sum_indirect += indirect_repair;
    candidate = min(candidate, repair - indirect_repair);
  }
  int repair = c[u] + sum_no_repair;
  int indirect_repair = min(repair, candidate + sum_indirect);
  int no_repair = min(indirect_repair, sum_indirect);
  return {no_repair, indirect_repair, repair};
}

int solve(int n, vector<vector<int>> &adj, vector<int> &c) {
  return get<1>(recsolve(n, adj, c, 0));
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; ++i) {
      int a, b;
      cin >> a >> b;
      adj[a].push_back(b);
    }
    vector<int> c(n);
    for (int i = 0; i < n; ++i) {
      cin >> c[i];
    }
    cout << solve(n, adj, c) << '\n';
  }
}