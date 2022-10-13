#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Point = K::Point_2;
using Line = K::Line_2;
using Seg = K::Segment_2;
using Triangle = K::Triangle_2;

using namespace std;

Line read_line() {
  int x1, y1, x2, y2;
  cin >> x1 >> y1 >> x2 >> y2;
  Point p1(x1, y1), p2(x2, y2);
  return Line(p1, p2);
}

Point read_point() {
  int x, y;
  cin >> x >> y;
  return Point(x, y);
}

Triangle read_triangle() {
  Line l1 = read_line();
  Line l2 = read_line();
  Line l3 = read_line();
  auto o1 = CGAL::intersection(l1, l2);
  auto o2 = CGAL::intersection(l1, l3);
  auto o3 = CGAL::intersection(l2, l3);
  const Point *p1 = boost::get<Point>(&*o1);
  const Point *p2 = boost::get<Point>(&*o2);
  const Point *p3 = boost::get<Point>(&*o3);
  assert(p1 && p2 && p3);
  return Triangle(*p1, *p2, *p3);
}

int main() {
  ios_base::sync_with_stdio(false);
  int c;
  cin >> c;
  while (c--) {
    int m, n;
    cin >> m >> n;
    vector<Point> points;
    points.reserve(m);
    for (int i = 0; i < m; ++i) {
      points.push_back(read_point());
    }
    vector<vector<bool>> intersects(n, vector<bool>(m));
    Triangle map;
    for (int i = 0; i < n; ++i) {
      map = read_triangle();
      for (int j = 0; j < m; ++j)
        intersects[i][j] = CGAL::do_intersect(map, points[j]);
    }
    auto covers = [&](int j, int k) {
      return intersects[j][k] && intersects[j][k + 1];
    };
    const int l = m - 1;
    vector<int> counts(l);
    int missing = l, ans = INT_MAX;
    for (int i = 0, j = 0; j < n; ++j) {
      for (int k = 0; k < l; ++k) {
        // +j
        if (covers(j, k)) {
          if (counts[k] == 0)
            --missing;
          ++counts[k];
        }
      }
      if (missing > 0)
        continue;
      while (i < j) {
        bool can = true;
        for (int k = 0; k < l; ++k) {
          if (covers(i, k) && counts[k] <= 1) {
            can = false;
            break;
          }
        }
        if (!can)
          break;
        for (int k = 0; k < l; ++k) {
          if (covers(i, k))
            --counts[k];
        }
        ++i;
      }
      ans = min(ans, j - i + 1);
    }
    cout << ans << endl;
  }
}