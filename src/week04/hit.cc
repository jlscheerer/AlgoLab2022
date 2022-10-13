#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using ll = long long;
using K = CGAL::Exact_predicates_inexact_constructions_kernel;

using namespace std;

K::Point_2 read_point() {
  ll x, y;
  cin >> x >> y;
  return K::Point_2((double)x, (double)y);
}

void test_case(int n) {
  K::Point_2 p1 = read_point(), p2 = read_point();
  K::Ray_2 proj(p1, p2);
  bool ix = false;
  for (int i = 0; i < n; ++i) {
    p1 = read_point(), p2 = read_point();
    K::Segment_2 seg(p1, p2);
    if (!ix)
      ix = CGAL::do_intersect(proj, seg);
  }
  if (ix)
    cout << "yes\n";
  else
    cout << "no\n";
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