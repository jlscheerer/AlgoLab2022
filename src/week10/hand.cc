#include <bits/stdc++.h>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/pending/disjoint_sets.hpp>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;

using Vb = CGAL::Triangulation_vertex_base_with_info_2<int, K>;
using Fb = CGAL::Triangulation_face_base_2<K>;
using Tds = CGAL::Triangulation_data_structure_2<Vb,Fb>;
using Delaunay = CGAL::Delaunay_triangulation_2<K,Tds>;

using Edge = tuple<int, int, long>;
using Point = K::Point_2;
using IPoint = std::pair<Point, int>;

// find the maximum number of families we can form of size >= k
int max_num_families(const int k, vector<int> &n_comp_k) {
  if (k == 1) {
    return n_comp_k[1];
  } else if (k == 2) {
    return n_comp_k[1] / 2 + n_comp_k[2];
  } else if (k == 3) {
    int c12 = min(n_comp_k[1], n_comp_k[2]);
    return c12 + (n_comp_k[1] - c12) / 3 + (n_comp_k[2] - c12) / 2 + n_comp_k[3];
  } else { // k == 4
    int c13 = min(n_comp_k[1], n_comp_k[3]);
    
    // "additional twos" (i.e., the ones remaining after 1 + 3)
    int a2 = (n_comp_k[1] - c13) / 2 + (n_comp_k[3] - c13);
    
    int c22 = (n_comp_k[2] + a2) / 2;
    return c13 + c22 + n_comp_k[4];
  }
}

pair<long, int> solve(int n, int k, int f0, long s0, vector<Edge> &edges) {
  boost::disjoint_sets_with_storage<> uf(n);
  int n_components = n;
  vector<int> s_components(n, 1); // size of components
  vector<int> n_comp_k(4 + 1); // number of components of size i
  n_comp_k[1] = n;
  long s = 0, f = -1;
  for (auto e = edges.begin(); e != edges.end(); ++e) {
    int c1 = uf.find_set(get<0>(*e));
    int c2 = uf.find_set(get<1>(*e));
    long dist = get<2>(*e);
    if (f == -1 && dist >= s0) { // all edges beyond this point are >= s0.
      f = max_num_families(k, n_comp_k);
    }
    if (c1 != c2) {
      // if we can still form at least f0 families of size k, we found a better answer
      if (max_num_families(k, n_comp_k) >= f0) {
        s = dist;
      }
      
      int s1 = s_components[c1], s2 = s_components[c2];
      uf.link(c1, c2);
      
      // new component id and size
      int cx = uf.find_set(c1), sx = s1 + s2;
      s_components[cx] = sx;
      
      // update component counts
      --n_comp_k[min(s1, k)], --n_comp_k[min(s2, k)], ++n_comp_k[min(sx, k)];
      
      if (--n_components == 0) break;
    }
  }
  return {s, f};
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    long n, k, f0, s0;
    cin >> n >> k >> f0 >> s0;
    vector<IPoint> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
      long x, y;
      cin >> x >> y;
      points.emplace_back(Point(x, y), i);
    }
    Delaunay t(points.begin(), points.end());
    vector<Edge> edges;
    edges.reserve(3 * n);
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      int i1 = e->first->vertex((e->second + 1) % 3)->info();
      int i2 = e->first->vertex((e->second + 2) % 3)->info();
      if (i1 > i2) swap(i1, i2);
      edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    sort(edges.begin(), edges.end(),
        [](const Edge& e1, const Edge& e2) -> bool {
          return get<2>(e1) < get<2>(e2);
    });
    long s; int f;
    tie(s, f) = solve(n, k, f0, s0, edges);
    cout << s << ' ' << f << '\n';
  }
}