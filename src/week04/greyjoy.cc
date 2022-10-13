#include <bits/stdc++.h>
#define ll long long

using namespace std;

// found, max
pair<bool, int> get_for(unordered_map<ll, tuple<int, int, int>> &m, int ri,
                        int cost) {
  if (!m.count(cost))
    return {false, INT_MIN / 4};
  auto found = m[cost];
  int pmax = get<0>(found), pwhere = get<1>(found), psmax = get<2>(found);
  if (pwhere == ri) {
    if (psmax > INT_MIN / 4)
      return {true, psmax};
    return {false, INT_MIN / 4};
  }
  return {true, pmax};
}

int solve_waterway(vector<ll> &c, vector<ll> &r, int ri,
                   unordered_map<ll, tuple<int, int, int>> &m, ll target) {
  const int n = r.size();
  int ans = 0;
  int j = n - 1;
  ll cost = 0;

  const auto update = [&](int i, int j, ll cost) {
    if (cost == target) {
      ans = max(ans, abs(i - j) + 1);
      return;
    } else if (cost < target && j <= 0) {
      auto other = get_for(m, ri, target - cost);
      if (other.first)
        ans = max(ans, abs(i) + 1 + other.second);
    }
  };

  for (; j >= 0; --j) {
    ll cur_cost = c[r[j]];
    cost += cur_cost;
    if (cost >= target)
      break;
  }
  update(n - 1, j, cost);
  for (int i = n - 2; i >= 0; --i) {
    cost -= c[r[i + 1]];
    while (j > 0 && cost < target)
      cost += c[r[--j]];
    update(i, j, cost);
  }
  return ans;
}

unordered_map<ll, tuple<int, int, int>> build_prefix(vector<ll> &c,
                                                     vector<vector<ll>> &rs) {
  // max where/-1(multiple) second_max(INT_MIN / 4)
  unordered_map<ll, tuple<int, int, int>> m;
  for (int ri = 0; ri < (int)rs.size(); ++ri) {
    const auto &r = rs[ri];
    ll cost = 0;
    for (int i = 1; i < (int)r.size(); ++i) { // exclude pyke
      cost += c[r[i]];
      int islands = i;
      if (m.count(cost)) {
        int pmax, pwhere, psmax;
        tie(pmax, pwhere, psmax) = m[cost];
        if (pmax > islands) {
          m[cost] = {pmax, pwhere, max(psmax, islands)};
        } else if (pmax == islands) {
          m[cost] = {pmax, -1,
                     pmax}; // found duplicate no need to track location
        } else if (pmax < islands) {
          m[cost] = {islands, ri, pmax};
        }
      } else {
        m[cost] = {islands, ri, INT_MIN / 4};
      }
    }
  }
  return m;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n, k, w;
    cin >> n >> k >> w;
    vector<ll> c(n);
    for (int i = 0; i < n; ++i)
      cin >> c[i];
    vector<vector<ll>> rs;
    for (int i = 0; i < w; ++i) {
      int l;
      cin >> l;
      rs.emplace_back(l);
      for (int j = 0; j < l; ++j)
        cin >> rs.back()[j];
    }
    unordered_map<ll, tuple<int, int, int>> m = build_prefix(c, rs);
    int ans = 0;
    for (int ri = 0; ri < w; ++ri) {
      ans = max(ans, solve_waterway(c, rs[ri], ri, m, k));
    }
    cout << ans << endl;
  }
}