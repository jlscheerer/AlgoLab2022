#include <bits/stdc++.h>

using namespace std;

using boat = pair<int, int>;
using indexed_boat = tuple<int, int, int>;

int boat_bound(const vector<boat> &boats, const indexed_boat &index) {
  int i = get<2>(index);
  return boats[i].second - boats[i].first;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n, l, p;
    cin >> n;
    vector<boat> boats;
    boats.reserve(n);
    for (int i = 0; i < n; ++i) {
      cin >> l >> p;
      boats.emplace_back(l, p);
    }
    // take the larger boat in case of ties!
    priority_queue<indexed_boat, vector<indexed_boat>, greater<>> gt, le;
    for (int i = 0; i < n; ++i) {
      gt.push({boats[i].second, -boats[i].first, i});
    }
    int ans = 0, bound = INT_MIN;
    while (gt.size() || le.size()) {
      // reshuffle (move from p - l > bound to bound >= p -l)
      while (gt.size() && bound >= boat_bound(boats, gt.top())) {
        auto move = gt.top();
        gt.pop();
        const int i = get<2>(move);
        le.push({boats[i].first, -boats[i].first, i});
      }
      indexed_boat candidate;
      if (!gt.size()) {
        candidate = le.top();
        le.pop();
      } else if (!le.size()) {
        candidate = gt.top();
        gt.pop();
      } else {
        int le_cand = get<2>(le.top());
        int gt_cand = get<2>(gt.top());
        if (bound + boats[le_cand].first < boats[gt_cand].second ||
            (bound + boats[le_cand].first == boats[gt_cand].second &&
             boats[le_cand].first > boats[gt_cand].first)) {
          candidate = le.top();
          le.pop();
        } else {
          candidate = gt.top();
          gt.pop();
        }
      }
      int id = get<2>(candidate);
      tie(l, p) = boats[id];
      if (bound > p)
        continue;
      ++ans;
      bound = max(bound, p - l) + l;
    }
    cout << ans << "\n";
  }
}