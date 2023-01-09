#include <bits/stdc++.h>

using namespace std;

int solve(int n, int m, long x, int k, vector<vector<pair<int, long>>> &adj) {
  vector<vector<long>> dp(k + 1, vector<long>(n, -1));
  dp[0][0] = 0;
  for (int move = 1; move <= k; ++move) {
    for (int u = 0; u < n; ++u) {
      if (dp[move - 1][u] == -1)
        continue;
      for (const auto &v_p : adj[u]) {
        int v = v_p.first;
        if (adj[v].size() == 0)
          v = 0; // directly move to start instead
        long p = v_p.second;
        long score = dp[move - 1][u] + p;
        if (score >= x)
          return move;
        dp[move][v] = max(dp[move][v], score);
      }
    }
  }
  return -1;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    long n, m, x, k;
    cin >> n >> m >> x >> k;
    vector<vector<pair<int, long>>> adj(n);
    for (int i = 0; i < m; ++i) {
      long u, v, p;
      cin >> u >> v >> p;
      adj[u].emplace_back(v, p);
    }
    int ans = solve(n, m, x, k, adj);
    if (ans == -1)
      cout << "Impossible\n";
    else
      cout << ans << '\n';
  }
}