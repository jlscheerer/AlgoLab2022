#include <bits/stdc++.h>

using namespace std;

int dfs(int n, vector<bool> &alive, int index) {
  if (index >= n || !alive[index])
    return 0;
  alive[index] = false;
  return 1 + dfs(n, alive, 2 * index + 1) + dfs(n, alive, 2 * index + 2);
}

bool solve(int n, vector<int> &t) {
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
  for (int i = 0; i < n; ++i) {
    pq.push({t[i], i});
  }
  vector<bool> alive(n, true);
  int curr = 0;
  while (pq.size()) {
    int tm, index;
    tie(tm, index) = pq.top();
    pq.pop();
    if (!alive[index])
      continue;
    curr += dfs(n, alive, index);
    if (curr > tm)
      return false;
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> t(n);
    for (int i = 0; i < n; ++i) {
      cin >> t[i];
    }
    if (solve(n, t))
      cout << "yes\n";
    else
      cout << "no\n";
  }
}