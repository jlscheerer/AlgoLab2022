#include <bits/stdc++.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Index = int;
using Vb = CGAL::Triangulation_vertex_base_with_info_2<Index, K>;
using Fb = CGAL::Triangulation_face_base_2<K>;
using Tds = CGAL::Triangulation_data_structure_2<Vb, Fb>;
using Triangulation = CGAL::Delaunay_triangulation_2<K, Tds>;

using Point = K::Point_2;
using IPoint = std::pair<Point, Index>;

using namespace std;

bool attempt(const int n, const long r, const int k, vector<IPoint> &planets) {
  boost::disjoint_sets_with_storage<> uf(n);
  // cannot take any of the first k planets
  const int safe_zone = k; // -1 ?
  Triangulation t(planets.begin() + safe_zone, planets.end());
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second + 1) % 3)->info();
    Index i2 = e->first->vertex((e->second + 2) % 3)->info();
    if (t.segment(e).squared_length() <= r * r) {
      uf.union_set(uf.find_set(i1), uf.find_set(i2));
    }
  }
  unordered_map<int, int> sizes;
  int max_ans = 0;
  for (int i = 0; i < n; ++i) {
    ++sizes[uf.find_set(i)];
    max_ans = max(max_ans, sizes[uf.find_set(i)]);
  }
  return max_ans >= k;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, r;
    cin >> n >> r;
    vector<IPoint> planets;
    planets.reserve(n);
    for (int i = 0; i < n; ++i) {
      int x, y;
      cin >> x >> y;
      planets.emplace_back(Point(x, y), i);
    }
    int left = 1, right = n, ans = 1 /* we can always do 1! */;
    while (left <= right) {
      int mid = left + (right - left) / 2;
      if (attempt(n, r, mid, planets)) {
        ans = mid;
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }
    cout << ans << '\n';
  }
}