#include <bits/stdc++.h>

using namespace std;

// index word_id
using word_occ = pair<int, int>;

int solve(const int n, vector<word_occ> &p) {
  vector<int> counts(n);
  // build initial
  int remaining = n, i = 0, j = 0;
  for (; i < (int)p.size() && remaining > 0; ++i) {
    const int word_id = p[i].second;
    if (counts[word_id] == 0) {
      --remaining;
    }
    ++counts[word_id];
    if (remaining == 0)
      break;
  }
  int ans = INT_MAX;
  auto collapse = [&]() {
    while (j < i && counts[p[j].second] > 1) {
      --counts[p[j].second];
      ++j;
    }
    ans = min(ans, p[i].first - p[j].first + 1);
  };
  collapse();

  ++i;
  // try "good" intervals
  for (; i < (int)p.size(); ++i) {
    const int word_id = p[i].second;
    ++counts[word_id];
    collapse();
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> occs(n);
    for (int i = 0; i < n; ++i) {
      cin >> occs[i];
    }
    vector<word_occ> p;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < occs[i]; ++j) {
        int pos;
        cin >> pos;
        p.emplace_back(pos, i);
      }
    }
    sort(p.begin(), p.end());
    cout << solve(n, p) << endl;
  }
}