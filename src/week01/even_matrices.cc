#include <bits/stdc++.h>

using namespace std;

constexpr int maxN = 200;
int mat[maxN][maxN];
int prefix[maxN][maxN];

int get_sum(int i, int j, int n) {
  if (i >= 0 && i < n && j >= 0 && j < n)
    return prefix[i][j];
  return 0;
}

int is_even(int i1, int i2, int j1, int j2, int n) {
  int sum = get_sum(i2, j2, n) - get_sum(i2, j1 - 1, n) -
            get_sum(i1 - 1, j2, n) + get_sum(i1 - 1, j1 - 1, n);
  return sum % 2 == 0;
}

void solve() {
  std::ios_base::sync_with_stdio(0);
  int n;
  cin >> n;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> mat[i][j];
      prefix[i][j] = mat[i][j];
      if (j > 0)
        prefix[i][j] += prefix[i][j - 1];
    }
  }
  for (int i = 1; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      prefix[i][j] += prefix[i - 1][j];
    }
  }

  int ans = 0;
  for (int i1 = 0; i1 < n; ++i1) {
    for (int i2 = i1; i2 < n; ++i2) {
      // solve like "even-pairs" O(n^2) -> O(n)
      int even = 0, odd = 0;
      for (int j = 0; j < n; ++j) {
        if (is_even(i1, i2, 0, j, n))
          ++even;
        else
          ++odd;
      }
      int even_even = (even * (even - 1)) / 2;
      int odd_odd = (odd * (odd - 1)) / 2;
      ans += even_even + odd_odd + even;
    }
  }
  cout << ans << endl;
}

int main() {
  int t;
  cin >> t;
  while (t--)
    solve();
}