#include <bits/stdc++.h>

using namespace std;

long solve(int n, int m, int k, vector<int> &v) {
  // [total, lowest_element]
  vector<vector<pair<int, int>>> dp(n + 1,
                                    vector<pair<int, int>>(m + 1, {-1, -1}));
  dp[0][0] = {0, 0};
  int j = 0, sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += v[i];
    while (j < i && sum > k)
      sum -= v[j++];
    const int len = i - j + 1;
    for (int c = 0; c <= m; ++c) {
      dp[i + 1][c] = dp[i][c];
      // extend an existing solution
      if (sum == k) {
        if (c == 0)
          continue;
        if (get<0>(dp[j][c - 1]) == -1)
          continue;
        if (c > 0 && get<1>(dp[j][c - 1]) < len) {
          if (get<0>(dp[i + 1][c]) < get<0>(dp[j][c - 1]) + len) {
            dp[i + 1][c] = {get<0>(dp[j][c - 1]) + len,
                            min(get<1>(dp[j][c - 1]), len)};
          }
        }
      }
    }
  }
  return get<0>(dp[n][m]);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
      cin >> v[i];
    }
    long ans = solve(n, m, k, v);
    if (ans < 0)
      cout << "fail\n";
    else
      cout << ans << '\n';
  }
}