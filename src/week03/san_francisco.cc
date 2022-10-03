#include <bits/stdc++.h>

using namespace std;

int main() {
  using edge = tuple<int, int, long long>;
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    long long x, k;
    cin >> n >> m >> x >> k;
    vector<int> outgoing(n);
    vector<edge> adj;
    adj.reserve(m);
    for (int i = 0; i < m; ++i) {
      int u, v;
      long long p;
      cin >> u >> v >> p;
      ++outgoing[u];
      // TODO we only care about the maximum
      adj.emplace_back(u, v, p);
    }
    vector<long long> max_d(n, -1), update(n, -1);
    max_d[0] = 0;
    long long ans = -1;
    for (int move = 1; move <= k; ++move) {
      long long max_score = 0;
      for (const auto &edge : adj) {
        int u = get<0>(edge), v = get<1>(edge);
        long long p = get<2>(edge);
        if (outgoing[v] == 0)
          v = 0;
        if (max_d[u] == -1)
          continue; // we did not reach this node before.
        update[v] = max(max_d[v], max(update[v], max_d[u] + p));
        max_score = max(max_score, update[v]);
      }
      if (max_score >= x) {
        ans = move;
        break;
      }
      swap(max_d, update);
    }
    if (ans == -1)
      cout << "Impossible\n";
    else
      cout << ans << "\n";
  }
}