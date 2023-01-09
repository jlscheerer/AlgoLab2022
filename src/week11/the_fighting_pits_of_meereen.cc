///1
#include <bits/stdc++.h>

using namespace std;

// (*) When compressing / Decompressing we don't need to keep track of the last
// element, i.e., it will be shifted out anyways!

// << [x_1, x_2, x_3] | int length (k^m)
// x_1 + x_2 * k + x_3 * k^2
class State {
public:
  static State decompress(int k, int m, int length, int compressed) {
    length = min(length, m);
    vector<int> x(length);
    for (int i = 0; i < length; ++i) {
      x[i] = compressed % k;
      compressed /= k;
    }
    return {k, m, x};
  }
  
  static int combine(int k, int m, const State &s1, const State &s2) {
    const int Memory = pow(k, m - 1); // (*)
    return s1.compress() + Memory * s2.compress();
  }
  
  int compress() const {
    int compressed = 0;
    int length = min((int)x_.size(), m_ - 1); // (*)
    for (int i = length - 1; i >= 0; --i) {
      compressed *= k_;
      compressed += x_[i];
    }
    return compressed;
  }
  
  int distinct() const {
    return set<int>(x_.begin(), x_.end()).size();
  }
  
  State operator<<(int new_x) const {
    int new_length = min((int)x_.size() + 1, m_);
    vector<int> new_xs(new_length);
    new_xs[0] = new_x;
    for (int i = 1; i < new_length; ++i) {
      new_xs[i] = x_[i - 1];
    }
    return {k_, m_, new_xs};
  }
  
private:
  State(int k, int m, vector<int> x)
    : k_(k), m_(m), x_(x) {}

  int k_, m_;
  vector<int> x_;
};

long solve(int n, int k, int m, vector<int> &x) {
  const int Memory = pow(k, m - 1); // (*)
  const int TotalMemory = Memory * Memory;
  constexpr int mxDiff = 11; // ceil(math.log2(4000))
  // n = 5 * 10^3, num_left = 5 * 10^3, state = (k ^ m) * (k ^ m) = 4096 = 4 * 10^3
  // vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(n + 1, vector<int>(TotalMemory, -1)));
  vector<vector<int>> dp_prev(2 * mxDiff + 1, vector<int>(TotalMemory, -1)),
                      dp_next(2 * mxDiff + 1, vector<int>(TotalMemory, -1));
  
  // dp[0][0][State::decompress(k, m, 0, 0).compress()] = 0;
  dp_prev[0 + mxDiff][State::decompress(k, m, 0, 0).compress()] = 0;
  int ans = -1;
  for (int i = 0; i < n; ++i) {
    const int xi = x[i];
    for (int diff = -mxDiff; diff <= mxDiff; ++diff) {
      // i = left + right, diff = left - right
      const int left = (i + diff) / 2;
      const int right = (i - diff) / 2;
      for (int mem = 0; mem < TotalMemory; ++mem) {
        const int dp_val = dp_prev[diff + mxDiff][mem];
        dp_prev[diff + mxDiff][mem] = -1; // clean-up for the next iteration
        if (dp_val <= -1) continue;
        
        const State left_mem = State::decompress(k, m, left, mem % Memory);
        const State right_mem = State::decompress(k, m, right, mem / Memory);
        
        // add to the left
        {
          const int new_diff = (left + 1) - right;
          if (new_diff >= -mxDiff && new_diff <= mxDiff) {
            // TODO(jlscheerer) Check that we don't exceed allowed diff...
            State new_left = (left_mem << xi);
            int &next = dp_next[new_diff + mxDiff][State::combine(k, m, new_left, right_mem)];
            next = max(next, dp_val + 1000 * new_left.distinct() - (1<<abs((left + 1) - right)));
            if (i == n - 1)  ans = max(ans, next);
          }
        }
        
        // add to the right
        {
          const int new_diff = left - (right + 1);
          if (new_diff >= -mxDiff && new_diff <= mxDiff) {
            // TODO(jlscheerer) Check that we don't exceed allowed diff...
            State new_right = (right_mem << xi);
            int &next = dp_next[new_diff + mxDiff][State::combine(k, m, left_mem, new_right)];
            next = max(next, dp_val + 1000 * new_right.distinct() - (1<<abs(left - (right + 1))));
            if (i == n - 1)  ans = max(ans, next);
          }
        }
        
      }
    }
    swap(dp_prev, dp_next);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, k, m;
    cin >> n >> k >> m;
    vector<int> x(n);
    for (int i = 0; i < n; ++i) {
      cin >> x[i];
    }
    cout << solve(n, k, m, x) << '\n';
  }
}