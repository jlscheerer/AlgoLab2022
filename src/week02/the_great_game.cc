#include <bits/stdc++.h>

using namespace std;

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    int r, b;
    cin >> r >> b;
    vector<pair<int, int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
      int u, v;
      cin >> u >> v;
      edges.emplace_back(u - 1, v - 1);
    }
    sort(edges.rbegin(), edges.rend());
    vector<int> mind(n, INT_MAX / 4); // minimum of maxds
    vector<int> maxd(n, INT_MIN / 4); // maximum of minds
    mind[n - 1] = maxd[n - 1] = 0;
    // compute min/max distance to "target"
    for (const auto edge : edges) {
      int i = edge.first, j = edge.second;
      if (mind[i] == -1) {
        mind[i] = maxd[j];
      }
      if (maxd[j] != -1)
        mind[i] = min(mind[i], maxd[j] + 1);
      if (maxd[i] == -1) {
        maxd[i] = mind[j];
      }
      if (mind[j] != -1)
        maxd[i] = max(maxd[i], mind[j] + 1);
    }
    int holmes = mind[r - 1], moriarty = mind[b - 1];

    if (holmes == moriarty) {
      cout << (holmes + 1) % 2 << '\n';
    } else if (holmes < moriarty) {
      cout << 0 << '\n';
    } else {
      cout << 1 << '\n';
    }
  }
}