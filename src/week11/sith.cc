#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Vb = CGAL::Triangulation_vertex_base_with_info_2<int, K>;
using Fb = CGAL::Triangulation_face_base_2<K>;
using Tds = CGAL::Triangulation_data_structure_2<Vb, Fb>;
using Delaunay = CGAL::Delaunay_triangulation_2<K, Tds>;

using Point = K::Point_2;
using IPoint = pair<Point, int>;

int solve(const int n, long r, vector<IPoint> &planets) {
  int left = 1, right = n / 2, ans = 0;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    Delaunay t(planets.begin() + mid, planets.end());
    boost::disjoint_sets_with_storage<> uf(n);
    vector<int> comp_sz(n, 1); // size of the components
    int lg_comp = 1; // size of the largest component
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      int i1 = e->first->vertex((e->second + 1) % 3)->info();
      int i2 = e->first->vertex((e->second + 2) % 3)->info();
      int c1 = uf.find_set(i1), c2 = uf.find_set(i2);
      K::FT dist = t.segment(e).squared_length();
      if (dist <= r * r && c1 != c2) {
        uf.link(c1, c2);
        int cx = uf.find_set(i1);
        comp_sz[cx] = comp_sz[c1] + comp_sz[c2];
        lg_comp = max(lg_comp, comp_sz[cx]);
      }
    }
    if (lg_comp >= mid) {
      ans = mid;
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    long n, r;
    cin >> n >> r;
    vector<IPoint> planets;
    planets.reserve(n);
    for (int i = 0; i < n; ++i) {
      long x, y;
      cin >> x >> y;
      planets.emplace_back(Point(x, y), i);
    }
    cout << solve(n, r, planets) << '\n';
  }
}