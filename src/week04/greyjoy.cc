#include <bits/stdc++.h>

using namespace std;

struct WayMap {
  void insert(int wi, long sum, int length) {
    if (m_.find(sum) == m_.end()) {
      m_[sum] = {length, wi, -1};
      return;
    }
    int mx, wj, smx;
    tie(mx, wj, smx) = m_[sum];
    if (length > mx) {
      smx = mx;
      mx = length;
      wj = wi;
    } else if (length == mx) {
      wj = -1; // no longer relevant.
    } else {
      smx = max(smx, length);
    }
    m_[sum] = {mx, wj, smx};
  }
  
  // find the maximum number of islands conquered with
  // num_poeple = sum & wi = != wj 
  int lookup(int wj, long sum) {
    if (m_.find(sum) == m_.end()) return -1;
    int mx, wi, smx;
    tie(mx, wi, smx) = m_[sum];
    if (wi == wj) return smx;
    return mx;
  }
  
private:
  // [max, wi, second_max]
  unordered_map<long, tuple<int, int, int>> m_;
};

int solve(int n, int k, int w, vector<int> &c, vector<vector<int>> &ways) {
  WayMap wm;
  for (int wi = 0; wi < w; ++wi) {
    long prefix = 0;
    for (int length = 0; length < (int)ways[wi].size(); ++length) {
      const int ri = ways[wi][length];
      prefix += c[ri];
      wm.insert(wi, prefix, length);
    }
  }
  int ans = 0;
  for (int wi = 0; wi < w; ++wi) {
    reverse(ways[wi].begin(), ways[wi].end());
    const int m = ways[wi].size();
    int j = 0; long sum = 0;
    for (int i = 0; i < m; ++i) {
      sum += c[ways[wi][i]];
      while (j < i && sum > k) {
        sum -= c[ways[wi][j]];
        ++j;
      }
      if (sum == k) {
        ans = max(ans, i - j + 1);
      }
    }
    while (j < m) {
      long target = k - sum + c[0];
      if (target > 0) {
        int other_length = wm.lookup(wi, target);
        if (other_length != -1) {
          ans = max(ans, m - j + other_length);
        }
      }
      sum -= c[ways[wi][j]];
      ++j;
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
    int n, k, w;
    cin >> n >> k >> w;
    vector<int> c(n);
    for (int i = 0; i < n; ++i) {
      cin >> c[i];
    }
    vector<vector<int>> ways(w);
    for (int i = 0; i < w; ++i) {
      int l;
      cin >> l;
      ways[i].reserve(l);
      for (int j = 0; j < l; ++j) {
        int r;
        cin >> r;
        ways[i].push_back(r);
      }
    }
    cout << solve(n, k, w, c, ways) << '\n';
  }
}