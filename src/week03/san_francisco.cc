#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using edge = tuple<int, int, long long>;

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n, m, u, v;
    ll x, k, p;
    cin >> n >> m >> x >> k;
    vector<int> out(n); // out[v]: number of outgoing edges for node v
    vector<edge> adj;   // adjacency list representing the input graph.
    adj.reserve(m);
    for (int i = 0; i < m; ++i) {
      cin >> u >> v >> p;
      ++out[u];
      adj.emplace_back(u, v, p);
    }
    vector<ll> dist_prev(n, -1); // dist[i - 1]
    vector<ll> dist_cur(n, -1);  // dist[i]
    dist_prev[0] = 0;            // initialize the source distance
    ll ans = -1;
    for (int move = 1; move <= k; ++move) {
      ll max_score = 0;
      for (const auto &edge : adj) {
        tie(u, v, p) = edge;
        // treat (u, v) with out[v] = 0 as (u, s)
        if (out[v] == 0)
          v = 0;
        // we did not reach this node before (i.e., dist[i - 1] = -∞)
        if (dist_prev[u] == -1)
          continue;
        // dist[i][v] = max {dist[i - 1][u] + w(u, v) | (u, v) ∈ E}
        dist_cur[v] = max(dist_prev[v], max(dist_cur[v], dist_prev[u] + p));
        max_score = max(max_score, dist_cur[v]);
      }
      if (max_score >= x) {
        ans = move;
        break;
      }
      swap(dist_prev, dist_cur);
    }
    if (ans != -1)
      cout << ans << "\n"; // we found a distance >= x in <= k moves.
    else
      cout << "Impossible\n"; // we did not find such a distance => impossible.
  }
}