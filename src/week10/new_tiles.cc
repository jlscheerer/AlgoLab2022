#include <bits/stdc++.h>

using namespace std;

int solve(int h, int w, vector<vector<int>> &grid) {
  if (h <= 1 || w <= 1) return 0;
  
  const auto is_compatible = [&](int mask, int i) {
    for(int j = 0; j < w; ++j){
      if((mask & (1 << j)) && !(grid[i][j] && grid[i-1][j]))
        return false;
    }
    return true;
  };
  
  vector<bool> valid(1 << w, true);
  for(int mask = 0; mask < (1 << w); ++mask){
    int count = 0;
    for (int i = 0; i < w; ++i) {
      if (mask & (1 << i)) {
        ++count;
      } else if (count % 2 != 0) {
        valid[mask] = false;
        break;
      }
    }
  }
  
  vector<vector<int>> dp(h, vector<int>(1 << w, 0));
  const auto unset = [&](int mask, int i) {
    int ans = 0;
    for (int j = 0; j < w; ++j) {
      if (mask & (1 << j)) {
        ans = max(ans, dp[i][mask - (1 << j)]);
      }
    }
    return ans;
  };
  
  for (int i = 1; i < h; ++i) {
    for (int mask = 0; mask < (1 << w); ++mask) {
      if (mask == 0) {
        dp[i][mask] = dp[i - 1][(1 << w) - 1];
        continue;
      }
      int max_without = unset(mask, i);
      int max_with = valid[mask] && is_compatible(mask, i) 
        ? dp[i - 1][((1 << w) - 1) ^ mask] + (__builtin_popcount(mask) / 2)
        : 0;
      dp[i][mask] = max(max_with, max_without);
    }
  }
  return dp[h - 1][(1 << w) - 1];
}


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int h, w, k;
    cin >> h >> w;
    vector<vector<int>> grid(h, vector<int>(w, 0));
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        cin >> k;
        if(k && (i > 0) && i < (h - 1) && (j > 0) && (j < (w - 1))){
            grid[i-1][j-1] = 1;
        }
      }
    }
    cout << solve(h - 2, w - 2, grid) << '\n';
  }
}