#include <bits/stdc++.h>

using namespace std;

using ll = long long;

unordered_map<int, vector<pair<ll, ll>>> split(vector<pair<ll, ll>> &M) {
  const int n = M.size();
  unordered_map<int, vector<pair<ll, ll>>> result;
  for (int pick = 0; pick < 1 << n; ++pick) {
    ll d = 0, t = 0;
    for (int i = 0; i < n; ++i) {
      if (pick & (1 << i)) {
        d += M[i].first;
        t += M[i].second;
      }
    }
    int moves = __builtin_popcount(pick);
    result[moves].emplace_back(t, d);
  }
  for (auto &moves_opts : result) {
    vector<pair<ll, ll>> &opts = moves_opts.second;
    sort(opts.begin(), opts.end());
    ll dist = 0;
    for (int i = 0; i < (int)opts.size(); ++i) {
      dist = max(dist, opts[i].second);
      opts[i].second = dist;
    }
  }
  return result;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    ll n, m, D, T;
    cin >> n >> m >> D >> T;
    vector<pair<ll, ll>> M1, M2;
    M1.reserve(n - n / 2);
    M2.reserve(n / 2);
    for (int i = 0; i < n; ++i) {
      ll d, t;
      cin >> d >> t;
      if (i % 2 == 0)
        M1.emplace_back(d, t);
      else
        M2.emplace_back(d, t);
    }
    vector<ll> s(m);
    for (int i = 0; i < m; ++i) {
      cin >> s[i];
    }
    int ans = INT_MAX;
    unordered_map<int, vector<pair<ll, ll>>> other = split(M2);
    const int m1 = M1.size();
    for (int pick = 0; pick < 1 << m1; ++pick) {
      ll d = 0, t = 0;
      for (int i = 0; i < m1; ++i) {
        if (pick & (1 << i)) {
          d += M1[i].first;
          t += M1[i].second;
        }
      }
      int moves = __builtin_popcount(pick);
      for (int other_moves = 0; other_moves <= 30; ++other_moves) {
        auto it =
            lower_bound(other[other_moves].begin(), other[other_moves].end(),
                        pair<ll, ll>{T - t, INT_MIN});
        int j;
        if (it == other[other_moves].end())
          j = other[other_moves].size() - 1;
        else
          j = (int)(it - other[other_moves].begin()) - 1;
        if (j < 0)
          continue;
        ll other_distance = other[other_moves][j].second;
        ll dist = d + other_distance;
        if (dist >= D) {
          ans = 0;
          break;
        }
        int total_moves = moves + other_moves;
        if (total_moves == 0)
          continue;
        ll remaining_distance = D - dist;
        ll inc_per_turn = (remaining_distance + total_moves - 1) / total_moves;
        auto it2 = lower_bound(s.begin(), s.end(), inc_per_turn);
        if (it2 == s.end())
          continue;
        ans = min(ans, (int)(it2 - s.begin()) + 1);
      }
      if (ans == 0) {
        break;
      }
    }
    if (ans < INT_MAX)
      cout << ans << "\n";
    else
      cout << "Panoramix captured\n";
  }
}