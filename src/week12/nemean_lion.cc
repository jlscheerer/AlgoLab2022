#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

using namespace std;

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Vb = CGAL::Triangulation_vertex_base_with_info_2<int, K>;
using Fb = CGAL::Triangulation_face_base_2<K>;
using Tds = CGAL::Triangulation_data_structure_2<Vb,Fb>;
using Delaunay = CGAL::Delaunay_triangulation_2<K, Tds>;

using Point = K::Point_2; 
using IPoint = pair<Point, int>;

tuple<int, int, int, int> solve(int n, long s, int h, vector<IPoint> &points) {
  Delaunay t(points.begin(), points.end());
  // returns the squared radius of the circumcircle of the given face.
  const auto get_face_radius = [&](Delaunay::Face_handle f) -> K::FT {
    if (t.is_infinite(f)) return K::FT(numeric_limits<long>::max());
    K::Circle_2 circumcircle(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
    return circumcircle.squared_radius();
  };
  
  // type: 0 - add, 1 - remove
  using Event = pair<K::FT, int>;
  vector<Event> events;
  
  int a_2 = 0, a_s = 0;
  auto min_radius_squared_2 = K::FT(numeric_limits<long>::max());
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    auto v_left = e->first->vertex((e->second + 1) % 3);
    auto v_right = e->first->vertex((e->second + 2) % 3);
    
    Point midpoint = CGAL::midpoint(v_left->point(), v_right->point());
    int v_nearest = t.nearest_vertex(midpoint)->info();
    
    Delaunay::Face_handle f1 = e->first, f2 = t.mirror_edge(*e).first;
    K::FT f1_radius = get_face_radius(f1), f2_radius = get_face_radius(f2);
    
    K::FT min_radius_squared, max_radius_squared = max(f1_radius, f2_radius);
    if (v_nearest != v_left->info() && v_nearest != v_right->info()) {
      min_radius_squared = min(f1_radius, f2_radius);
    } else {
      min_radius_squared = t.segment(*e).squared_length() / 4;
    }
    events.emplace_back(min_radius_squared, 0);
    events.emplace_back(max_radius_squared, 1);
    
    if (K::FT(s) >= min_radius_squared && K::FT(s) <= max_radius_squared) {
      ++a_s;
    }
    
    if (min_radius_squared < min_radius_squared_2) {
      min_radius_squared_2 = min_radius_squared;
      a_2 = 1;
    } else if (min_radius_squared == min_radius_squared_2) {
      ++a_2;
    }
  }

  int a_3 = 0;
  auto min_radius_squared_3 = get_face_radius(t.finite_faces_begin());
  for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
    auto radius_squared = get_face_radius(f);
    if (radius_squared < min_radius_squared_3) {
      min_radius_squared_3 = radius_squared;
      a_3 = 1;
    } else if (radius_squared == min_radius_squared_3) {
      ++a_3;
    }
  }
  
  sort(events.begin(), events.end());
  int a_max = 0, curr = 0;
  for (const auto &event: events) {
    if (event.second == 0) ++curr;
    else --curr;
    a_max = max(a_max, curr);
  }
  return {a_2, a_3, a_s, a_max};
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    long n, s, h;
    cin >> n >> s >> h;
    vector<IPoint> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
      long x, y;
      cin >> x >> y;
      points.emplace_back(Point(x, y), i);
    }
    int a_2, a_3, a_s, a_max;
    tie(a_2, a_3, a_s, a_max) = solve(n, s, h, points);
    cout << a_2 << ' ' << a_3 << ' ' << a_s << ' ' << a_max << '\n';
  }
}