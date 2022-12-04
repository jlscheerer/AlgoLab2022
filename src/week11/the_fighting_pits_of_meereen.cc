#include <bits/stdc++.h>

using namespace std;

constexpr int mxDiff = 12;
constexpr int mxK = 4;
constexpr int mxM = 3;

// | i1 | i2 * (k)^1 | [i3 * (k)^3]?
class Memory {
public:
  Memory() {}
  Memory operator<<(int next) const {
    array<int, mxM> new_items;
    for (int i = mxM - 1; i >= 1; --i) {
      new_items[i] = items_[i - 1];
    }
    new_items[0] = next;
    return {m_, k_, min(size_ + 1, m_), new_items};
  }
  int distinct() const {
    int ans = 0;
    for (int i = 0; i < size_; ++i) {
      bool uniq = true;
      for (int j = 0; j < i; ++j) {
        uniq = uniq && (items_[i] != items_[j]);
      }
      ans += uniq;
    }
    return ans;
  }
  int compress() const {
    int compressed = 0;
    for (int i = size_ - 1; i >= 0; --i) {
      compressed *= k_;
      compressed += items_[i];
    }
    return compressed;
  }
  static int compress(Memory left, Memory right) {
    int m = left.m_, k = left.k_;
    const int offset = pow(k, m);
    // IDEA: Last element will be shifted out anyways... (can remove it during
    // compression)
    return (left.compress() % (int)pow(k, m - 1)) +
           offset * (right.compress() % (int)pow(k, m - 1));
  }
  static Memory decompress(int m, int k, int size, int compressed) {
    array<int, mxM> items;
    for (int i = 0; i < mxM; ++i) {
      items[i] = compressed % k;
      compressed /= k;
    }
    return {m, k, size, items};
  }

private:
  Memory(int m, int k, int size, array<int, mxM> items)
      : m_(m), k_(k), size_(min(size, m)), items_(items) {
    for (int i = size; i < mxM; ++i) {
      items_[i] = 0;
    }
  }

  int m_, k_, size_;
  array<int, mxM> items_;
};

int solve(const int n, const int k, const int m, vector<int> &f) {
  const int entrance_memory = pow(k, m);
  const int total_memory = entrance_memory * entrance_memory;
  vector<vector<int>> dp_prev(2 * mxDiff + 1, vector<int>(total_memory, -1)),
      dp_curr(2 * mxDiff + 1, vector<int>(total_memory, -1));
  dp_prev[0 + 12][0] = 0;
  Memory left, right;
  int ans = 0;
  for (int i = 0; i < n; ++i) { // n = #left + #right
    const int fighter = f[i];
    for (int diff = -mxDiff; diff <= mxDiff; ++diff) { // #right - #left
      for (int mem = 0; mem < total_memory; ++mem) {
        const int value = dp_prev[diff + mxDiff][mem];
        dp_prev[diff + mxDiff][mem] = -1; // clean-up
        if (value < 0)
          continue; // unreachable state
        // n - diff = #left + #right - (#right - #left) = 2 * #left
        const int num_left = (i - diff) / 2;
        // n + diff = #left + #right + (#right - #left) = 2 * #right
        const int num_right = (i + diff) / 2;

        left = Memory::decompress(m, k, num_left, mem % entrance_memory);
        right = Memory::decompress(m, k, num_right, mem / entrance_memory);

        if (diff != -mxDiff) { // enter from the left
          const int new_diff = diff - 1;
          const int new_mem = Memory::compress((left << fighter), right);
          int &new_value = dp_curr[new_diff + mxDiff][new_mem];
          new_value =
              max(new_value, value + 1000 * (left << fighter).distinct() -
                                 (1 << abs(new_diff)));
          if (i == n - 1)
            ans = max(ans, new_value);
        }

        if (diff != mxDiff) { // enter from the right
          const int new_diff = diff + 1;
          const int new_mem = Memory::compress(left, (right << fighter));
          int &new_value = dp_curr[new_diff + mxDiff][new_mem];
          new_value =
              max(new_value, value + 1000 * (right << fighter).distinct() -
                                 (1 << abs(new_diff)));
          if (i == n - 1)
            ans = max(ans, new_value);
        }
      }
    }
    swap(dp_prev, dp_curr);
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
    vector<int> f(n);
    for (int i = 0; i < n; ++i) {
      cin >> f[i];
    }
    int ans = solve(n, k, m, f);
    cout << ans << '\n';
  }
}