#include <bits/stdc++.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Vb = CGAL::Triangulation_vertex_base_2<K>;
using Fb = CGAL::Triangulation_face_base_with_info_2<int, K>;
using Tds = CGAL::Triangulation_data_structure_2<Vb, Fb>;
using Delaunay = CGAL::Delaunay_triangulation_2<K, Tds>;

using Point = K::Point_2;

constexpr int INFINITE_FACE = 0;

struct Edge {
  int neighbor;
  long distance;
};

struct Area {
  Area(const int n, vector<Point> &points) : t(points.begin(), points.end()) {
    int curr_id = 1;
    for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
      f->info() = t.is_infinite(f) ? INFINITE_FACE : curr_id++;
    }

    unordered_map<int, vector<Edge>> adj;
    for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
      for (int neighbor = 0; neighbor < 3; ++neighbor) {
        long distance =
            CGAL::squared_distance(f->vertex((neighbor + 1) % 3)->point(),
                                   f->vertex((neighbor + 2) % 3)->point());
        int neighbor_id = f->neighbor(neighbor)->info();
        adj[f->info()].push_back({neighbor_id, distance});

        // also store neighbors of "infinite" face.
        if (neighbor_id == INFINITE_FACE) {
          adj[INFINITE_FACE].push_back({f->info(), distance});
        }
      }
    }

    dp = vector<long>(curr_id, -1);
    priority_queue<pair<long, int>> pq;
    pq.push({LONG_MAX, INFINITE_FACE});
    while (pq.size()) {
      long distance = pq.top().first;
      int face_id = pq.top().second;
      pq.pop();
      if (dp[face_id] >= distance)
        continue;
      dp[face_id] = distance;
      for (const auto &edge : adj[face_id]) {
        int neighbor_id = edge.neighbor;
        long upd_distance = min(distance, edge.distance);
        if (dp[neighbor_id] >= distance)
          continue;
        pq.push({upd_distance, neighbor_id});
      }
    }
  }

  bool can_escape(Point p, long d) {
    long dist = CGAL::squared_distance(p, t.nearest_vertex(p)->point());
    return (dist >= d && dp[t.locate(p)->info()] >= 4 * d);
  }

private:
  Delaunay t;
  vector<long> dp;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  while (true) {
    int n;
    cin >> n;
    if (n == 0)
      break;
    vector<Point> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
      long x, y;
      cin >> x >> y;
      points.emplace_back(x, y);
    }
    Area area(n, points);
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
      long x, y, d;
      cin >> x >> y >> d;
      if (area.can_escape(Point(x, y), d))
        cout << 'y';
      else
        cout << 'n';
    }
    cout << '\n';
  }
}