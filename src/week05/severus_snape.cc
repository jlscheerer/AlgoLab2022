#include <bits/stdc++.h>

using namespace std;

struct Potion {
  long p, h;
};

vector<long> compute_dp(int n, int H, vector<Potion> &potions) {
  // dp[k][h][i]: max power we can get
  //                - using exactly k items
  //                - having happyness >= h
  //                - using potions in the range [..i]
  vector<vector<vector<long>>> dp(
      n + 1, vector<vector<long>>(H + 1, vector<long>(n + 1)));
  for (int k = 1; k <= n; ++k) {
    for (int h = 0; h <= H; ++h) {
      for (int i = 1; i <= n; ++i) {
        const auto &p = potions[i - 1];
        long no_take = dp[k][h][i - 1];
        long take = dp[k - 1][max(h - p.h, 0l)][i - 1];
        if (take > 0 || p.h >= h)
          take += p.p;
        dp[k][h][i] = max(no_take, take);
      }
    }
  }
  vector<long> ans(n + 1);
  for (int k = 0; k <= n; ++k) {
    ans[k] = dp[k][H][n];
  }
  return ans;
}

int solve(int n, int m, long a, long b, long P, long H, long W,
          vector<Potion> &potions_a, vector<long> &potions_b) {
  vector<long> dp_a = compute_dp(n, H, potions_a);

  sort(potions_b.rbegin(), potions_b.rend());
  vector<long> dp_b(m + 1);
  for (int i = 0; i < m; ++i) {
    dp_b[i + 1] = dp_b[i] + potions_b[i];
  }

  int ans = INT_MAX;
  for (int sa = 0; sa <= n; ++sa) {
    for (int sb = 0; sb <= m; ++sb) {
      long Px = dp_a[sa] - sb * b;
      long Wx = dp_b[sb] - sa * a;
      if (Px >= P && Wx >= W) {
        ans = min(ans, sb + sa);
      }
    }
  }
  return ans == INT_MAX ? -1 : ans;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    long n, m, a, b, P, H, W;
    cin >> n >> m >> a >> b >> P >> H >> W;
    vector<Potion> potions_a;
    potions_a.reserve(n);
    for (int i = 0; i < n; ++i) {
      long p, h;
      cin >> p >> h;
      potions_a.push_back({p, h});
    }
    vector<long> potions_b(m);
    for (int i = 0; i < m; ++i) {
      cin >> potions_b[i];
    }
    cout << solve(n, m, a, b, P, H, W, potions_a, potions_b) << '\n';
  }
}