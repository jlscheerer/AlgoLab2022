#include <bits/stdc++.h>

using namespace std;

struct NodeInfo {
  int id;
  long gold; // number of gold when starting from t = 0
  long children, time;
};

void recsolve(vector<NodeInfo> &dp, vector<long> &g, unordered_map<int, vector<pair<int, int>>> &adj, int u) {
  long gold = 0, num_children = 0, time = 0, v, l;
  vector<NodeInfo> children;
  unordered_map<int, long> child_costs;
  for (const auto &v_l: adj[u]) {
    tie(v, l) = v_l;
    recsolve(dp, g, adj, v);
    num_children += dp[v].children + 1;
    time += dp[v].time + l;
    children.push_back(dp[v]);
    child_costs[v] = l;
   }
  sort(children.begin(), children.end(), [&](const auto &a, const auto &b) {
    return (1 + a.children) * (child_costs[b.id] + b.time) > (1 + b.children) * (child_costs[a.id] + a.time);
  });
  int time_spent = 0;
  for (const auto &child: children) {
    time_spent += child_costs[child.id];
    gold += child.gold - (1 + child.children) * time_spent;
    time_spent += child_costs[child.id] + 2 * child.time;
  }
  // cout << u << " " << (gold + g[u]) << endl;
  dp[u] = {u, gold + g[u], num_children, time};
}

long solve(const int n, vector<long> &g, unordered_map<int, vector<pair<int, int>>> &adj) {
  vector<NodeInfo> dp(n + 1);
  recsolve(dp, g, adj, 0);
  return dp[0].gold;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<long> g(n + 1);
    for (int i = 1; i <= n; ++i) {
      cin >> g[i];
    }
    unordered_map<int, vector<pair<int, int>>> adj;
    for (int i = 0; i < n; ++i) {
      int u, v, l;
      cin >> u >> v >> l;
      adj[u].emplace_back(v, l);
    }
    cout << solve(n, g, adj) << '\n';
  }
}