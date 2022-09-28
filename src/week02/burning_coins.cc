#include <bits/stdc++.h>

using namespace std;

int solve(vector<int> &v, vector<vector<int>> &dp, int i, int j) {
  int n = j - i;
  if (n <= 0)
    return 0;
  if (dp[i][j] != -1)
    return dp[i][j];
  if (n == 1)
    return dp[i][j] = v[i];
  int ans = 0;

  // pick last
  int ans2 = min(solve(v, dp, i + 1, j - 1), solve(v, dp, i, j - 2));
  ans = max(ans, ans2 + v[j - 1]);

  // pick first
  ans2 = min(solve(v, dp, i + 2, j), solve(v, dp, i + 1, j - 1));
  ans = max(ans, ans2 + v[i]);

  return dp[i][j] = ans;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> v;
    v.reserve(n);
    for (int i = 0; i < n; ++i) {
      cin >> v[i];
    }
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    cout << solve(v, dp, 0, n) << endl;
  }
}