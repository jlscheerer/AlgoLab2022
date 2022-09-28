#include <bits/stdc++.h>

using namespace std;

pair<int, int> solve(int n, int k, vector<int> &v) {
  // lambda to (possibly) update the answer
  int ans_i = -1, ans_j = -1, ans_sum = INT_MAX;
  auto update = [&](int i, int j, int ans) {
    if (abs(ans - k) < abs(ans_sum - k) ||
        (abs(ans - k) == abs(ans_sum - k) &&
         ((i < ans_i) || (i == ans_i && j < ans_j)))) {
      ans_sum = ans;
      ans_i = i;
      ans_j = j;
    }
  };

  int sum = 0;
  for (int i = 0, j = 0; j < n; ++j) {
    sum += v[j];
    update(i, j, sum);
    while (i < j && sum > k) {
      sum -= v[i++];
      update(i, j, sum);
    }
  }
  return {ans_i, ans_j};
}

int main() {
  int t;
  cin >> t;
  vector<int> v;
  while (t--) {
    int n, k;
    cin >> n >> k;
    v.resize(n);
    for (int i = 0; i < n; ++i)
      cin >> v[i];
    auto ans = solve(n, k, v);
    cout << ans.first << " " << ans.second << "\n";
  }
}