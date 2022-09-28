#include <bits/stdc++.h>

using namespace std;

void solve(vector<int> &x) {
  const int n = x.size();
  x.push_back(INT_MAX / 4);
  int i = 0, j = 0;
  while (i < n && x[i + 1] - x[j] <= 200)
    ++i;
  auto distance = [&]() {
    return max(abs(x[i] - (x[i] + x[j]) / 2), abs(x[j] - (x[i] + x[j]) / 2));
  };
  int max_p = j - i + 1, max_d = distance();
  set<int> locations;
  for (; i < n; ++i) {
    while (x[i] - x[j] > 200)
      ++j;
    int para = i - j + 1;
    int dist = distance();
    if (para > max_p || (para == max_p && dist < max_d)) {
      max_p = para;
      max_d = dist;
      locations.clear();
    }
    if (para == max_p && dist == max_d) {
      int c1 = (x[i] + x[j] - 1) / 2, c2 = (x[i] + x[j]) / 2,
          c3 = (x[i] + x[j] + 1) / 2;
      if (max(abs(x[i] - c1), abs(x[j] - c1)) == dist) {
        locations.insert(c1);
      }
      if (max(abs(x[i] - c2), abs(x[j] - c2)) == dist) {
        locations.insert(c2);
      }
      if (max(abs(x[i] - c3), abs(x[j] - c3)) == dist) {
        locations.insert(c3);
      }
    }
  }
  cout << max_p << " " << max_d << '\n';
  for (int x : locations)
    cout << x << " ";
  cout << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> x(n);
    for (int i = 0; i < n; ++i) {
      cin >> x[i];
    }
    sort(x.begin(), x.end());
    solve(x);
  }
}