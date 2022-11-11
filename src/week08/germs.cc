#include <bits/stdc++.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>

using EPIC = CGAL::Exact_predicates_inexact_constructions_kernel;
using Triangulation = CGAL::Delaunay_triangulation_2<EPIC>;
using Edge_iterator = Triangulation::Finite_edges_iterator;
using Vertex_iterator = Triangulation::Finite_vertices_iterator;
using Vertex_handle = Triangulation::Vertex_handle;

using Point = EPIC::Point_2;

using namespace std;

using ll = long long;

ll find_t(ll dist) {
  ll t = 0;
  while (dist > 4 * (t * t * t * t) + 4 * (t * t) + 1)
    ++t;
  return t;
}

void solve(int n) {
  int l, b, r, t;
  cin >> l >> b >> r >> t;
  vector<Point> points;
  points.reserve(n);

  unordered_map<int, unordered_map<int, ll>> max_dist;
  auto update = [&](Point p, ll dist, ll prev_def = 0) {
    int x = p.x(), y = p.y();
    if (max_dist[x].find(y) == max_dist[x].end()) {
      max_dist[x][y] = dist;
      return false;
    } else {
      ll prev = max_dist[x][y];
      max_dist[x][y] = min(dist, min(prev, prev_def));
      return true;
    }
  };
  int duplicates = 0;
  for (int i = 0; i < n; ++i) {
    int x, y;
    cin >> x >> y;
    Point point(x, y);
    points.push_back(point);
    ll dist = min(min(x - l, r - x), min(y - b, t - y));
    duplicates += update(point, 4 * dist * dist);
  }
  Triangulation triag(points.begin(), points.end());
  for (Edge_iterator e = triag.finite_edges_begin();
       e != triag.finite_edges_end(); ++e) {
    Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
    Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
    ll dist = CGAL::squared_distance(
        v1->point(), v2->point()); // (2 * dist) ^ 2 = 4 * dist^2
    update(v1->point(), dist, LLONG_MAX);
    update(v2->point(), dist, LLONG_MAX);
  }
  vector<int> times(duplicates);
  times.reserve(n);
  for (const auto &x_ps : max_dist) {
    for (const auto &y_d : x_ps.second) {
      ll dist = y_d.second;
      times.push_back(find_t(dist));
    }
  }
  sort(times.begin(), times.end());
  cout << times[0] << ' ' << times[times.size() / 2] << ' '
       << times[times.size() - 1] << '\n';
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