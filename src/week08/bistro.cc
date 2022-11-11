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

Point read_point() {
  int x, y;
  cin >> x >> y;
  return Point(x, y);
}

void solve(int n) {
  vector<Point> points;
  points.reserve(n);
  for (int i = 0; i < n; ++i) {
    points.push_back(read_point());
  }
  Triangulation t;
  t.insert(points.begin(), points.end());
  int m;
  cin >> m;
  for (int i = 0; i < m; ++i) {
    Point point = read_point();
    Point nearest = t.nearest_vertex(point)->point();
    ll dist = CGAL::squared_distance(point, nearest);
    cout << dist << '\n';
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  while (true) {
    int n;
    cin >> n;
    if (!n)
      break;
    solve(n);
  }
}