#include <bits/stdc++.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>

using EPIC = CGAL::Exact_predicates_inexact_constructions_kernel;
using Triangulation = CGAL::Delaunay_triangulation_2<EPIC>;
using Face_iterator = Triangulation::Finite_faces_iterator;

using Point = EPIC::Point_2;

using namespace std;

using ll = long long;

struct Participant {
  Point point;
  int radius;
};

bool is_hit(Triangulation &t, const Participant &p, ll h) {
  Point nearest = t.nearest_vertex(p.point)->point();
  ll dist = CGAL::squared_distance(p.point, nearest);
  return dist < (h + p.radius) * (h + p.radius);
}

bool attempt(vector<Participant> &participants, vector<Point> &lights, ll h,
             int count) {
  Triangulation t(lights.begin(), lights.begin() + count);
  for (const auto &p : participants) {
    if (!is_hit(t, p, h))
      return true;
  }
  return false;
}

vector<int> compute(vector<Participant> &participants, vector<Point> &lights,
                    ll h, int count) {
  vector<int> ans;
  Triangulation t(lights.begin(), lights.begin() + count);
  for (int i = 0; i < (int)participants.size(); ++i) {
    if (!is_hit(t, participants[i], h))
      ans.push_back(i);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int m, n;
    cin >> m >> n;
    vector<Participant> participants;
    participants.reserve(m);
    for (int i = 0; i < m; ++i) {
      int x, y, r;
      cin >> x >> y >> r;
      participants.push_back({Point(x, y), r});
    }
    ll h;
    cin >> h;
    vector<Point> lights;
    lights.reserve(n);
    for (int i = 0; i < n; ++i) {
      int x, y;
      cin >> x >> y;
      lights.emplace_back(x, y);
    }
    int left = 0, right = n, ans = -1;
    while (left <= right) {
      int mid = (left + right + 1) / 2;
      if (attempt(participants, lights, h, mid)) {
        left = mid + 1;
        ans = mid;
      } else {
        right = mid - 1;
      }
    }
    vector<int> last = compute(participants, lights, h, ans);
    for (int x : last)
      cout << x << ' ';
    cout << '\n';
  }
}