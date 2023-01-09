#include <bits/stdc++.h>

using namespace std;

long solve(int n, vector<int> &a, vector<int> &b) {
  vector<vector<int>> dp(n + 1, vector<int>(n + 1, INT_MAX / 8));
  dp[0][0] = 0;
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      // Observation one of both cases will take exactly one element!
      // i.e., either we will take only a single "a" or a single "b".
      long pred = min(dp[i - 1][j], min(dp[i][j - 1], dp[i - 1][j - 1]));
      if (pred >= INT_MAX / 8) continue;
      dp[i][j] = pred + (a[i - 1] - 1) * (b[j - 1] - 1);
    }
  }
  return dp[n][n];
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; ++i) {
      cin >> a[i];
    }
    for (int i = 0; i < n; ++i) {
      cin >> b[i];
    }
    cout << solve(n, a, b) << '\n';
  }
}