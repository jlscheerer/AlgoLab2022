#include <bits/stdc++.h>

using namespace std;

int solve(int n, vector<int> &h) {
  int back = h[0], ans = 1;
  for (int i = 1; i < n; ++i) {
    if (i < back) {
      back = max(back, i + h[i]);
      ++ans;
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