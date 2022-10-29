#include <bits/stdc++.h>

using namespace std;

struct Interval {
  int begin, end;
  int length() const { return end - begin; }
};

struct Boat {
  int length, position;
  bool operator<(const Boat &other) {
    return (position < other.position) ||
           (position == other.position && length < other.length);
  }
  int starting_position() const { return position - length; }
};

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n, l, p;
    cin >> n;
    vector<Boat> boats;
    for (int i = 0; i < n; ++i) {
      cin >> l >> p;
      boats.push_back({/*length=*/l, /*position=*/p});
    }
    sort(boats.begin(), boats.end()); // sort by (position, length)
    int ans = 1;
    Interval prev = {-1, -1}, curr = {/*begin=*/-1, /*end=*/boats[0].position};
    for (int i = 1; i < (int)boats.size(); ++i) {
      if (curr.end < boats[i].position) {
        // it does not hurt to take `curr` (remaining boats will fit)
        prev = curr, ++ans;
        curr.begin = max(prev.end, boats[i].starting_position());
        curr.end = curr.begin + boats[i].length;
      } else if (boats[i].length < curr.length()) {
        // we found a boat that fits (instead of curr) & ends earlier!
        curr.begin = max(prev.end, boats[i].starting_position());
        curr.end = curr.begin + boats[i].length;
      }
    }
    cout << ans << '\n';
  }
}