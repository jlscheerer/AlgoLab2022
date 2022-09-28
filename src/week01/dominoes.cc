#include <bits/stdc++.h>

using namespace std;

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
      cin >> v[i];
    }
    if (!v.size()) {
      cout << 0 << endl;
      continue;
    }
    int ans = 1, cur = v[0] - 1;
    for (int i = 1; i < n; ++i) {
      if (cur <= 0)
        break;
      ++ans;
      cur = max(cur, v[i]);
      --cur;
    }
    cout << ans << endl;
  }
}