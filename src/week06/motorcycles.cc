#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <bits/stdc++.h>

using EPEC = CGAL::Exact_predicates_exact_constructions_kernel;
using Point = EPEC::Point_2;
using Ray = EPEC::Ray_2;
using Line = EPEC::Line_2;

using namespace std;
using ll = long; // must use long here!

// y0 x1 y1 id
using rider = tuple<ll, ll, ll, int>;

Ray rider_ray(const rider &r) {
  Point p0((ll)0, get<0>(r)), p1(get<1>(r), get<2>(r));
  return Ray(p0, p1);
}

Line rider_line(const rider &r) {
  int flip = (get<0>(r) < get<2>(r)) ? 1 : -1;
  return Line(Point(0, flip * get<0>(r)), Point(get<1>(r), flip * get<2>(r)));
}

bool winner(const rider &a, const rider b) {
  auto result = CGAL::compare_slope(rider_line(a), rider_line(b));
  if (result != CGAL::EQUAL)
    return result == CGAL::SMALLER;
  return get<0>(a) < get<0>(b);
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<rider> riders;
    riders.reserve(n);
    for (int i = 0; i < n; ++i) {
      ll y0, x1, y1;
      cin >> y0 >> x1 >> y1;
      riders.emplace_back(y0, x1, y1, i);
    }
    sort(riders.begin(), riders.end());
    vector<bool> removed(n);
    for (int i = 0; i < n; ++i) {
      for (int j = i - 1; j >= 0; --j) {
        if (removed[get<3>(riders[j])])
          continue;
        if (!CGAL::do_intersect(rider_ray(riders[i]), rider_ray(riders[j]))) {
          break;
        }
        int win = winner(riders[i], riders[j]);
        if (win == 1) { // i wins
          removed[get<3>(riders[j])] = true;
        } else { // j wins
          removed[get<3>(riders[i])] = true;
          break;
        }
      }
    }
    for (int i = 0; i < n; ++i) {
      if (removed[i])
        continue;
      cout << i << " ";
    }
    cout << endl;
  }
}
