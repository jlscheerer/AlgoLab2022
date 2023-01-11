#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

using namespace std;

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using  Vb = CGAL::Triangulation_vertex_base_with_info_2<int, K>;
using Fb = CGAL::Triangulation_face_base_2<K>;
using Tds = CGAL::Triangulation_data_structure_2<Vb, Fb>;
using Delaunay = CGAL::Delaunay_triangulation_2<K, Tds>;
using Vertex_handle = Tds::Vertex_handle;

using Point = K::Point_2;
using IPoint = pair<Point, int>;

using graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;

unordered_set<int> vertices_in_radius(Delaunay& t, Vertex_handle u, long r) {
  unordered_set<int> ans;
  vector<Vertex_handle> q;
  q.push_back(u);
  ans.insert(u->info());
  while (q.size()) {
    auto v = q.back(); q.pop_back();
    auto neigh = v->incident_vertices();
    do {
      if (!t.is_infinite(neigh) && !ans.count(neigh->info())
          && CGAL::squared_distance(u->point(), neigh->point()) <= r * r) {
        ans.insert(neigh->info());
        q.push_back(neigh);
      }
    } while(++neigh != v->incident_vertices());
  }
  return ans;
}

struct Network {
  Network(const int n, const int m, long r, vector<IPoint> &stations) 
   : t(stations.begin(), stations.end()), r(r), components(n) {
    graph G(n);
    for (auto u = t.finite_vertices_begin(); u != t.finite_vertices_end(); ++u) {
      unordered_set<int> vs = vertices_in_radius(t, u, r);
      for (int v: vs) {
        if (u->info() != v)
          boost::add_edge(u->info(), v, G);
      }
    }
    has_interference = !boost::is_bipartite(G);
    boost::connected_components(G, boost::make_iterator_property_map(components.begin(), boost::get(boost::vertex_index, G)));
  }
  
  bool can_route(Point a, Point b) const {
    if (has_interference) return false;
    if (CGAL::squared_distance(a, b) <= r * r) return true;
    auto va = t.nearest_vertex(a), vb = t.nearest_vertex(b);
    if (CGAL::squared_distance(a, va->point()) > r * r
        || CGAL::squared_distance(b, vb->point()) > r * r) 
      return false;
    return components[va->info()] == components[vb->info()];
  }
  
private:
  Delaunay t;
  long r;
  bool has_interference;
  vector<int> components;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    long n, m, r;
    cin >> n >> m >> r;

    vector<IPoint> stations;
    stations.reserve(n);
    for (int i = 0; i < n; ++i) {
      long x, y;
      cin >> x >> y;
      stations.emplace_back(Point(x, y), i);
    }
    
    Network network(n, m, r, stations);
    for (int i = 0; i < m; ++i) {
      long ax, ay, bx, by;
      cin >> ax >> ay >> bx >> by;
      Point a(ax, ay), b(bx, by);
      if (network.can_route(a, b)) cout << 'y';
      else cout << 'n';
    }
    cout << '\n';
  }
}