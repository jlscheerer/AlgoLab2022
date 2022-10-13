#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using K = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;
using Traits = CGAL::Min_circle_2_traits_2<K>;
using MinCircle = CGAL::Min_circle_2<Traits>;

using namespace std;

template <typename T> long long ceil_to_ll(const T &x) {
  double a = ceil(CGAL::to_double(x));
  while (a < x)
    a += 1;
  while (a - 1 >= x)
    a -= 1;
  return (long long)a;
}

int main() {
  ios_base::sync_with_stdio(false);
  while (true) {
    int n;
    cin >> n;
    if (!n)
      break;
    vector<K::Point_2> P;
    P.reserve(n);
    for (int i = 0; i < n; ++i) {
      long xi, yi;
      cin >> xi >> yi;
      P.emplace_back(xi, yi);
    }
    MinCircle mc(P.begin(), P.end(), true);
    Traits::Circle c = mc.circle();
    cout << ceil_to_ll(CGAL::sqrt(c.squared_radius())) << "\n";
  }
}