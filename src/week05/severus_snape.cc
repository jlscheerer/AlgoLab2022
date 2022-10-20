#include <bits/stdc++.h>

using namespace std;

using ll = long long;

vector<vector<vector<ll>>> compute_dp(int H, vector<pair<ll, ll>> &potions) {
  const int m = potions.size();
  // dp[k][h][i]: max power we can get
  //                - using exactly k items
  //                - having happy >= h
  //                - using potions in the range [..i]
  vector<vector<vector<ll>>> dp(m + 1,
                                vector<vector<ll>>(H + 1, vector<ll>(m + 1)));
  for (int k = 1; k <= m; ++k) {
    for (int h = 0; h <= H; ++h) {
      for (int i = 0; i < m; ++i) {
        ll power, happy;
        const auto &potion = potions[i];
        tie(power, happy) = potion;
        ll no_take = dp[k][h][i];
        ll take = dp[k - 1][max(h - happy, (ll)0)][i];
        if (take > 0 || max(h - happy, (ll)0) == 0)
          take += power; // can we actually take it?
        dp[k][h][i + 1] = max(no_take, take);
      }
    }
  }
  return dp;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    ll n, m, a, b, P, H, W;
    cin >> n >> m >> a >> b >> P >> H >> W;
    vector<pair<ll, ll>> potion_a;
    potion_a.reserve(n);
    for (int i = 0; i < n; ++i) {
      ll p, h;
      cin >> p >> h;
      potion_a.emplace_back(p, h);
    }
    vector<vector<vector<ll>>> dp = compute_dp(H, potion_a);
    vector<ll> potion_b(m);
    for (int i = 0; i < m; ++i) {
      cin >> potion_b[i];
    }
    sort(potion_b.rbegin(), potion_b.rend());
    int ans = INT_MAX;
    for (int sb = 0; sb <= m; ++sb) {
      ll power = 0, happy = 0, wit = 0;
      for (int i = 0; i < sb; ++i) {
        wit += potion_b[i];
      }
      int j = 0;
      while (j < n && (power < (P + sb * b) || happy < H)) {
        power = dp[j + 1][H][n];
        happy = H;
        ++j;
      }
      if (power >= (P + sb * b) && happy >= H && wit >= W + j * a) {
        ans = min(ans, sb + j);
      }
    }
    cout << (ans == INT_MAX ? -1 : ans) << "\n";
  }
}