#include <bits/stdc++.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Triangulation = CGAL::Delaunay_triangulation_2<Kernel>;
using Edge_iterator = Triangulation::Edge_iterator;
using Face_iterator = Triangulation::Finite_faces_iterator;

using namespace std;

using Point = Kernel::Point_2;

struct Edge {
  int neighbor;
  long distance;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  while (true) {
    int n, m, x, y;
    long d;
    cin >> n;
    if (!n)
      break;
    vector<Point> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
      cin >> x >> y;
      points.emplace_back(x, y);
    }
    Triangulation t(points.begin(), points.end());
    const int INFINITE_FACE = 0;
    unordered_map<void *, int> ids;
    ids[nullptr] = INFINITE_FACE; // reserve "INFINITE_FACE"
    auto id_face = [&](auto f) {
      if (t.is_infinite(f))
        return INFINITE_FACE;
      void *addr = &*f;
      if (ids.find(addr) == ids.end()) {
        ids[addr] = ids.size();
      }
      return ids[addr];
    };

    unordered_map<int, vector<Edge>> adj;
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end();
         ++f) {
      int face_id = id_face(f);
      for (int neighbor = 0; neighbor < 3; ++neighbor) {
        long distance =
            CGAL::squared_distance(f->vertex((neighbor + 1) % 3)->point(),
                                   f->vertex((neighbor + 2) % 3)->point());
        int neighbor_id = id_face(f->neighbor(neighbor));
        adj[face_id].push_back({neighbor_id, distance});
        if (neighbor_id ==
            INFINITE_FACE) { // also store neighbors of "infinite" face.
          adj[INFINITE_FACE].push_back({face_id, distance});
        }
      }
    }

    vector<long> dp(ids.size(), -1);
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

    cin >> m;
    for (int i = 0; i < m; ++i) {
      cin >> x >> y >> d;
      Point point(x, y);
      long default_distance =
          CGAL::squared_distance(point, t.nearest_vertex(point)->point());
      if (default_distance < d) {
        cout << 'n';
        continue;
      }
      auto face = t.locate(point);
      int face_id = id_face(face);
      long required_distance = dp[face_id];
      if (required_distance < 4 * d) { // (2 * d)^2 = 4d^2
        cout << 'n';
      } else {
        cout << 'y';
      }
    }
    cout << '\n';
  }
}