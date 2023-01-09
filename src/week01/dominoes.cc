#include <bits/stdc++.h>

using namespace std;

int solve(int n, vector<int> &h) {
  int ans = 0, bound = 1;
  for (int i = 0; i < n; ++i) {
    if (bound > i) {
      ++ans;
      bound = max(bound, i + h[i]);
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
    int n;
    cin >> n;
    vector<int> h(n);
    for (int i = 0; i < n; ++i) {
      cin >> h[i];
    }
    cout << solve(n, h) << '\n';
  }
}