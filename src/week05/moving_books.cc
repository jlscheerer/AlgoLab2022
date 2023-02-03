#include <bits/stdc++.h>

using namespace std;

int solve(int n, int m, vector<int> &s, vector<int> &w) {
  sort(s.rbegin(), s.rend());
  sort(w.rbegin(), w.rend());
  if (s.front() < w.front())
    return -1;

  const auto can = [&](const int k) {
    if (k == 0 || (m + k - 1) / k > n)
      return false;
    for (int j = 0; j < m; ++j) {
      if (s[j / k] < w[j])
        return false;
    }
    return true;
  };

  int left = 1, right = m, ans = -1;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (can(mid)) {
      right = mid - 1;
      ans = mid;
    } else {
      left = mid + 1;
    }
  }

  return 3 * ans - 1;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    vector<int> s(n);
    for (int i = 0; i < n; ++i) {
      cin >> s[i];
    }
    vector<int> w(m);
    for (int i = 0; i < m; ++i) {
      cin >> w[i];
    }
    int ans = solve(n, m, s, w);
    if (ans == -1)
      cout << "impossible\n";
    else
      cout << ans << '\n';
  }
}