#include <bits/stdc++.h>

using namespace std;

bool can_place_mask(int row, int mask) {
  // for every 1 in mask we must have a 1 in row
  return ((~row) & mask) == 0;
}

bool is_valid_domino_mask(const int w, int mask) {
  // we must have even number of consecutive ones
  if (__builtin_popcount(mask) % 2 != 0)
    return false;
  int count = 0;
  for (int i = 0; i < w; ++i) {
    if (mask & (1 << i))
      ++count;
    else if (count % 2 != 0)
      return false;
    else
      count = 0;
  }
  return count % 2 == 0;
}

bool domino_compatible(int prev, int curr) {
  // prev needs zero everywhere curr has a one
  return ((~prev) & curr) == curr;
}

int solve(const int h, const int w, vector<int> &grid) {
  vector<vector<int>> dp(h, vector<int>(2 << w, INT_MIN));
  for (int bitmask = 0; bitmask < 2 << w; ++bitmask) {
    dp[0][bitmask] = 0;
  }
  for (int i = 1; i < h; ++i) {
    for (int bitmask = 0; bitmask < 2 << w; ++bitmask) {
      if (!can_place_mask(grid[i], bitmask))
        continue;
      if (!can_place_mask(grid[i - 1], bitmask))
        continue;
      if (!is_valid_domino_mask(w, bitmask))
        continue;

      for (int prev_mask = 0; prev_mask < 2 << w; ++prev_mask) {
        if (!domino_compatible(prev_mask, bitmask))
          continue;
        if (dp[i - 1][prev_mask] == INT_MIN)
          continue;
        dp[i][bitmask] =
            max(dp[i][bitmask],
                dp[i - 1][prev_mask] + (__builtin_popcount(bitmask)) / 2);
      }
    }
  }
  int ans = 0;
  for (int bitmask = 0; bitmask < 2 << w; ++bitmask) {
    if (dp[h - 1][bitmask] == INT_MIN)
      continue;
    ans = max(ans, dp[h - 1][bitmask]);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int h, w;
    cin >> h >> w;
    vector<int> grid(h);
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        int x;
        cin >> x;
        grid[i] = grid[i] << 1 | x;
      }
    }
    int ans = solve(h, w, grid);
    cout << ans << '\n';
  }
}