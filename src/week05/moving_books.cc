#include <bits/stdc++.h>

using namespace std;
constexpr int mxN = 1e6;

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    vector<int> s(n), w(m);
    for (int i = 0; i < n; ++i) {
      cin >> s[i];
    }
    sort(s.begin(), s.end());
    for (int i = 0; i < m; ++i) {
      cin >> w[i];
    }
    sort(w.begin(), w.end());
    if (w[m - 1] > s[n - 1]) {
      cout << "impossible\n";
      continue;
    }
    vector<int> workload(n);
    int j = 0, prev = 0;
    for (int i = 0; i < n; ++i) {
      while (j < m && s[i] >= w[j])
        ++j;
      workload[i] = j - prev, prev = j;
      if (j >= m)
        break;
    }
    int left = 0, right = mxN;
    int ans = 0;
    while (left <= right) {
      int mid = (left + right + 1) / 2;
      int overflow = 0;
      for (int i = 0; i < n; ++i) {
        int actual = workload[i] + overflow;
        overflow = max(actual - mid, 0);
      }
      if (overflow == 0) {
        ans = mid;
        right = mid - 1;
      } else {
        left = mid + 1;
      }
    }
    cout << (ans - 1) * 3 + 2 << endl;
  }
}