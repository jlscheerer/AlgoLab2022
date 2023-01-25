#include <bits/stdc++.h>

using namespace std;

long solve(int n, vector<int> &a, vector<int> &b) {
  vector<vector<long>> dp(n + 1, vector<long>(n + 1, INT_MAX));
  dp[0][0] = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      // Observation one of both cases will take exactly one element!
      // i.e., either we will take only a single "a" or a single "b".
      dp[i + 1][j + 1] = min(dp[i][j + 1], min(dp[i + 1][j], dp[i][j])) +
                         (a[i] - 1) * (b[j] - 1);
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
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
      cin >> a[i];
    }
    vector<int> b(n);
    for (int i = 0; i < n; ++i) {
      cin >> b[i];
    }
    cout << solve(n, a, b) << '\n';
  }
}