#include <bits/stdc++.h>

using namespace std;

struct Movement {
  long d, t;
  
  bool operator<(const Movement &other) {
    return pair<long, long>{t, d} < pair<long, long>{other.t, other.d};
  }
};

// ans[i]: sorted (t asc, d monotonically inc.) list of "combined" movements taking i turns
vector<vector<Movement>> split_and_list(vector<Movement> &M) {
  const int n = M.size();
  vector<vector<Movement>> ans(n + 1);
  for (int config = 0; config < 1 << n; ++config) {
    int num_moves = __builtin_popcount(config);
    long d = 0, t = 0;
    for (int i = 0; i < n; ++i) {
      if (config & (1 << i)) {
        d += M[i].d;
        t += M[i].t;
      }
    }
    ans[num_moves].push_back({d, t});
  }
  for (int i = 0; i <= n; ++i) {
    sort(ans[i].begin(), ans[i].end());
    // want to make the moves monotonically increasing, so that we can binary search later.
    long max_dist = 0;
    for (int j = 0; j < (int)ans[i].size(); ++j) {
      max_dist = max(max_dist, ans[i][j].d);
      ans[i][j].d = max_dist;
    }
  }
  return ans;
}

int solve(const int n, const int m, long D, long T, vector<Movement> &M, vector<long> s) {
  // Split & List: Split the possible Movements
  vector<Movement> M1(M.begin(), M.begin() + n / 2);
  const int m1 = M1.size();
  
  vector<Movement> M2(M.begin() + n / 2, M.end());
  const int m2 = M2.size();
  vector<vector<Movement>> split = split_and_list(M2);
  
  int ans = INT_MAX;
  for (int config = 0; config < 1 << m1; ++config) {
    int num_moves = __builtin_popcount(config);
    long d = 0, t = 0;
    for (int i = 0; i < m1; ++i) {
      if (config & (1 << i)) {
        d += M1[i].d;
        t += M1[i].t;
      }
    }
    for (int other_moves = 0; other_moves <= m2; ++other_moves) {
      const int total_moves = num_moves + other_moves;
      if (total_moves == 0) continue;
      
      // find the maximial distance we can in the remaining time taking other_moves moves.
      Movement BOUND{INT_MIN, T - t};
      auto it = lower_bound(split[other_moves].begin(), split[other_moves].end(), BOUND);
      int j = distance(split[other_moves].begin(), it) - 1;
      if (j < 0) continue;
      
      // calculate the minimum "potion strength" we would need to make this work.
      long rem_dist = D - d - split[other_moves][j].d;
      long inc_per_turn = (rem_dist + total_moves - 1) / total_moves;
      
      // do we have such a potion?
      auto s_it = lower_bound(s.begin(), s.end(), inc_per_turn);
      if (s_it != s.end()) {
        ans = min(ans, (int)distance(s.begin(), s_it));
      }
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    long n, m, D, T;
    cin >> n >> m >> D >> T;
    vector<Movement> M;
    M.reserve(n);
    for (int i = 0; i < n; ++i) {
      long d, t;
      cin >> d >> t;
      M.push_back({d, t});
    }
    vector<long> s(m + 1);
    for (int i = 0; i < m; ++i) {
      cin >> s[i + 1];
    }
    int ans = solve(n, m, D, T, M, s);
    if (ans == INT_MAX) cout << "Panoramix captured\n";
    else cout << ans << '\n';
  }
}