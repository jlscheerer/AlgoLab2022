#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using ll = long long;
using EPEC = CGAL::Exact_predicates_exact_constructions_kernel;
using Point = EPEC::Point_2;
using Ray = EPEC::Ray_2;
using Seg = EPEC::Segment_2;
using FT = EPEC::FT;

using namespace std;

template <typename T> ll floor_to_ll(const T &x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x)
    a -= 1;
  while (a + 1 <= x)
    a += 1;
  return a;
}

Point read_point() {
  ll x, y;
  cin >> x >> y;
  return Point((double)x, (double)y);
}

Point compute_distance(const Point &p1, const Ray &ray, const Seg &seg) {
  auto o = CGAL::intersection(ray, seg);
  if (Point *ix = boost::get<Point>(&*o)) {
    return *ix;
  } else if (Seg *s = boost::get<Seg>(&*o)) {
    auto d1 = CGAL::squared_distance(p1, s->source());
    auto d2 = CGAL::squared_distance(p1, s->target());
    if (d1 < d2) {
      return s->source();
    } else {
      return s->target();
    }
  }
  // this should NEVER happen!
  return {};
}

void test_case(int n) {
  Point p1 = read_point(), p2 = read_point(), ans;
  Ray proj2(p1, p2);
  bool found = false;
  const auto exact_distance = [&](EPEC::Segment_2 &seg) {
    ans = compute_distance(p1, proj2, seg);
    found = true;
  };
  vector<Seg> segments;
  segments.reserve(n);
  for (int i = 0; i < n; ++i) {
    Point s1 = read_point();
    Point s2 = read_point();
    segments.emplace_back(s1, s2);
  }
  random_shuffle(segments.begin(), segments.end());
  int i = 0;
  for (; i < (int)segments.size(); ++i) {
    if (CGAL::do_intersect(proj2, segments[i])) {
      exact_distance(segments[i]);
      break;
    }
  }
  if (!found) {
    cout << "no\n";
    return;
  }
  for (; i < (int)segments.size(); ++i) {
    Seg ray(p1, ans); // make sure it is at least as good as the current answer.
    if (CGAL::do_intersect(ray, segments[i])) {
      exact_distance(segments[i]);
    }
  }
  cout << floor_to_ll(ans.x()) << " " << floor_to_ll(ans.y()) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  while (true) {
    int n;
    cin >> n;
    if (!n)
      break;
    test_case(n);
  }
}