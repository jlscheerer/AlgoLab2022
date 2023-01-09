#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point = K::Point_2;

istream &operator>>(istream &is, Point &p) {
  long x, y;
  cin >> x >> y;
  p = Point(x, y);
  return is;
}

struct Triangle {
  Triangle(array<Point, 6> &points)
    : points(points) {}
  
  bool intersect(const Point &p) const {
    for (int i = 0; i < 3; ++i) {
      const Point &p0 = points[2 * i], &p1 = points[2 * i + 1], &q = points[(2 * i + 2) % 6];
      auto op = CGAL::orientation(p0, p1, p);
      if (op == CGAL::COLLINEAR) continue;
      auto ot = CGAL::orientation(p0, p1, q);
      if (op != ot) return false;
    }
    return true;
  }
  
private:
  array<Point, 6> points;
};

int solve(const int m, const int n, vector<Point> &points, vector<Triangle> &maps) {
  vector<vector<bool>> contains(n, vector<bool>(m));
  for (int i = 0; i < n; ++i) {
    const auto &t = maps[i];
    for (int j = 0; j < m; ++j) {
      contains[i][j] = t.intersect(points[j]);
    }
  }
  const int k = m - 1; // number of paths
  const auto is_contained = [&](int i, int j) {
    return contains[i][j] && contains[i][j + 1];
  };
  int n_rem = k, j = 0, ans = INT_MAX;
  vector<int> cnts(k);
  for (int i = 0; i < n; ++i) {
    // add the "map i"
    for (int kk = 0; kk < k; ++kk) {
      if (is_contained(i, kk)) {
        ++cnts[kk];
        if (cnts[kk] == 1) --n_rem;
      }
    }
    // remove the "map j"?
    while (j < i && n_rem == 0) {
      bool can = true;
      for (int kk = 0; kk < k; ++kk) {
        if (is_contained(j, kk) && cnts[kk] == 1) {
          can = false;
          break;
        }
      }
      if (!can) break;
      for (int kk = 0; kk < k; ++kk) {
        cnts[kk] -= is_contained(j, kk);
      }
      ++j;
    }
    if (n_rem == 0) ans = min(ans, i - j + 1);
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
    vector<Point> points(m);
    for (int i = 0; i < m; ++i) {
      cin >> points[i];
    }
    vector<Triangle> maps;
    maps.reserve(n);
    for (int i = 0; i < n; ++i) {
      array<Point, 6> map_points;
      for (int i = 0; i < 6; ++i)
        cin >> map_points[i];
      maps.emplace_back(map_points);
    }
    cout << solve(m, n, points, maps) << '\n';
  }
}