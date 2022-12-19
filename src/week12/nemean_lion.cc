#include <bits/stdc++.h>

using namespace std;

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2, Index> IPoint;

using Point = K::Point_2;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    long n, s, h;
    cin >> n >> s >> h;
    vector<Point> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
      int x, y;
      cin >> x >> y;
      points.emplace_back(x, y);
    }
    Delaunay t(points.begin(), points.end());
    int a2 = 0, a3 = 1, as = h, amax = h;

    // a2 := number of edges in Delaunay with smallest distance
    {
      K::FT smallest_dist = LONG_MAX;
      int count = 0;
      for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        auto length = t.segment(e).squared_length();
        if (length < smallest_dist) {
          smallest_dist = length;
          count = 1;
        } else if (length == smallest_dist) {
          ++count;
        }
      }
      a2 = count;
    }

    // a3 := number of faces in Delaunay with the smallest "radius"
    {
      K::FT smallest_dist = LONG_MAX;
      int count = 0;
      for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
        Point center = t.dual(f);
        auto radius = CGAL::squared_distance(center, f->vertex(0)->point());
        if (radius < smallest_dist) {
          smallest_dist = radius;
          count = 1;
        } else if (radius == smallest_dist) {
          ++count;
        }
      }
      a3 = count;
    }

    {
      K::FT smallest_dist = LONG_MAX;
      for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        // Index i1 = e->first->vertex((e->second+1)%3)->info();
        // Index i2 = e->first->vertex((e->second+2)%3)->info();
        // t.segment(e).midpoint();
      }
    }

    cout << a2 << ' ' << a3 << ' ' << as << ' ' << amax << '\n';
  }
}