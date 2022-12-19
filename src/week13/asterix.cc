#include <bits/stdc++.h>

using namespace std;

// direct: city i is repaired;
// indirect: city i is repaired (indirectly), i.e., by repairing one of its
// children; no_repair: city i has not been repaired and, therefore, still needs
// to be repaired.
tuple<int, int, int> recsolve(unordered_map<int, vector<int>> &children,
                              vector<int> &costs, int u) {
  if (children[u].size() == 0) { // leaf?
    return {costs[u], costs[u], 0};
  }
  vector<pair<int, int>> child_costs;
  child_costs.reserve(children[u].size());
  int sum_indirect = 0, sum_no_repair = 0;
  for (const auto &v : children[u]) {
    int direct, indirect, no_repair;
    tie(direct, indirect, no_repair) = recsolve(children, costs, v);
    child_costs.push_back({direct, indirect});
    sum_indirect += indirect;
    sum_no_repair += no_repair;
  }
  // direct: current_cost + sum(no_repair_children_costs)
  // indirect: direct_any_child + sum(indirect_children_costs \ child)
  // no_repair: sum(indirect_children_costs)
  // no_repair <= indirect <= direct.
  const int direct = costs[u] + sum_no_repair;
  int indirect = INT_MAX;
  for (const auto &d_i : child_costs) {
    int d, i;
    tie(d, i) = d_i;
    indirect = min(indirect, sum_indirect - i + d);
  }
  indirect = min(direct, indirect);
  const int no_repair = min(indirect, sum_indirect);

  return {direct, indirect, no_repair};
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    unordered_map<int, vector<int>> children;
    for (int i = 0; i < n - 1; ++i) {
      int x, y;
      cin >> x >> y;
      children[x].push_back(y);
    }
    vector<int> costs(n);
    for (int i = 0; i < n; ++i) {
      cin >> costs[i];
    }
    int direct, indirect, no_repair;
    tie(direct, indirect, no_repair) = recsolve(children, costs, 0);
    cout << min(direct, indirect) << '\n';
  }
}