#include <bits/stdc++.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Index = int;
using Vb = CGAL::Triangulation_vertex_base_with_info_2<Index, Kernel>;
using Fb = CGAL::Triangulation_face_base_2<Kernel>;
using Tds = CGAL::Triangulation_data_structure_2<Vb, Fb>;
using Triangulation = CGAL::Delaunay_triangulation_2<Kernel, Tds>;

using namespace std;

using Point = Kernel::Point_2;
using IPoint = pair<Point, Index>;

bool dfs(unordered_map<int, vector<int>> &adj, vector<int> &colors, int u,
         int c) {
  if (colors[u] != 0) {
    return colors[u] == c;
  }
  colors[u] = c;
  for (int v : adj[u]) {
    if (!dfs(adj, colors, v, -c))
      return false;
  }
  return true;
}

bool can_color(const int n, unordered_map<int, vector<int>> &adj) {
  vector<int> colors(n);
  for (int i = 0; i < n; ++i) {
    if (colors[i] != 0)
      continue;
    if (!dfs(adj, colors, i, 1))
      return false;
  }
  return true;
}

void build_adj(unordered_map<int, vector<int>> &adj, deque<IPoint> &window,
               double r) {
  IPoint &pj = window.back();
  for (int i = window.size() - 2; i >= 0; --i) {
    const IPoint &pi = window[i];
    if (CGAL::squared_distance(pi.first, pj.first) <= r * r) {
      adj[pi.second].push_back(pj.second);
      adj[pj.second].push_back(pi.second);
    }
  }
}

bool has_interferences(const int n, vector<IPoint> &stations, double r) {
  sort(stations.begin(), stations.end(), [](const IPoint &a, const IPoint &b) {
    return a.first.x() < b.first.x();
  });
  unordered_map<int, vector<int>> adj;
  deque<IPoint> window;
  for (int i = 0; i < n; ++i) {
    window.push_back(stations[i]);
    while ((window.back().first.x() - window.front().first.x()) > r) {
      window.pop_front();
    }
    build_adj(adj, window, r);
  }
  build_adj(adj, window, r);
  return !can_color(n, adj);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    double r;
    cin >> n >> m >> r;
    vector<IPoint> stations;
    stations.reserve(n);
    for (int i = 0; i < n; ++i) {
      int x, y;
      cin >> x >> y;
      stations.emplace_back(Point{x, y}, i);
    }
    bool interferences = has_interferences(n, stations, r);

    Triangulation t(stations.begin(), stations.end());
    boost::disjoint_sets_with_storage<> uf(n);
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      Index i1 = e->first->vertex((e->second + 1) % 3)->info();
      Index i2 = e->first->vertex((e->second + 2) % 3)->info();
      double dist = t.segment(e).squared_length();
      if (dist <= r * r) {
        Index c1 = uf.find_set(i1), c2 = uf.find_set(i2);
        uf.link(c1, c2);
      }
    }

    for (int i = 0; i < m; ++i) {
      int ax, ay, bx, by;
      cin >> ax >> ay >> bx >> by;
      if (interferences) {
        cout << 'n';
        continue;
      }
      Point a(ax, ay), b(bx, by);
      long dist_ab = CGAL::squared_distance(a, b);
      if (dist_ab <= r * r) {
        // other radio set is in range
        cout << 'y';
        continue;
      }
      auto sa = t.nearest_vertex(a), sb = t.nearest_vertex(b);
      long da = CGAL::squared_distance(a, sa->point());
      long db = CGAL::squared_distance(b, sb->point());
      if (da <= r * r && db <= r * r &&
          (uf.find_set(sa->info()) == uf.find_set(sb->info()))) {
        cout << 'y';
        continue;
      }
      cout << 'n';
    }
    cout << '\n';
  }
}