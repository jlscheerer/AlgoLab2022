#include <bits/stdc++.h>

using namespace std;

long solve(const int n, vector<int> &a, vector<int> &b) {
  vector<vector<long>> dp(n + 1, vector<long>(n + 1, LONG_MAX / 8));
  dp[0][0] = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      // Observation one of both cases will take exactly one element!
      // i.e., either we will take only a single "a" or a single "b".
      long prev_cost = dp[i][j];
      {
        long a_part = 0;
        int i2 = i;
        a_part += a[i2];
        long b_part = 0;
        for (int j2 = j; j2 < n; ++j2) {
          b_part += b[j2];
          dp[i2 + 1][j2 + 1] =
              min(dp[i2 + 1][j2 + 1], prev_cost + a_part * b_part);
        }
      }
      {
        long b_part = 0;
        int j2 = j;
        b_part += b[j2];
        long a_part = 0;
        for (int i2 = i; i2 < n; ++i2) {
          a_part += a[i2];
          dp[i2 + 1][j2 + 1] =
              min(dp[i2 + 1][j2 + 1], prev_cost + a_part * b_part);
        }
      }
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
      --a[i];
    }
    for (int i = 0; i < n; ++i) {
      cin >> b[i];
      --b[i];
    }
    long ans = solve(n, a, b);
    cout << ans << '\n';
  }
}