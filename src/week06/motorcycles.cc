#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Point = K::Point_2;
using Ray = K::Ray_2;

struct Biker {
  Biker(int index, long y0, long x1, long y1)
    : index(index), slope(K::FT(y1 - y0) / K::FT(x1)),  ray(Point(0, y0), Point(x1, y1)) {}
  
  K::FT y() const {
    return ray.source().y();
  }
  
  bool intersect(const Biker &other) const {
    return CGAL::do_intersect(ray, other.ray);
  }
  
  int index;
  K::FT slope;
  Ray ray;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<Biker> bikers;
    bikers.reserve(n);
    for (int i = 0; i < n; ++i) {
      long y0, x1, y1;
      cin >> y0 >> x1 >> y1;
      bikers.emplace_back(i, y0, x1, y1);
    }
    sort(bikers.begin(), bikers.end(), [](const auto &b1, const auto &b2) {
      return b1.y() > b2.y();
    });
    vector<bool> alive(n);
    int j = 0; // last alive biker
    alive[bikers[j].index] = true;
    for (int i = 1; i < n; ++i) {
      if (bikers[i].slope <= bikers[j].slope) {
        j = i;
        alive[bikers[j].index] = true;
        continue;
      }
      if (abs(bikers[i].slope) <= abs(bikers[j].slope)) {
        while (j >= 0 && (!alive[bikers[j].index] || 
            (abs(bikers[i].slope) <= abs(bikers[j].slope) && bikers[i].intersect(bikers[j])))) {
          alive[bikers[j].index] = false;
          --j;
        }
        if (j == - 1 || bikers[i].slope <= bikers[j].slope) {
          j = i;
          alive[bikers[j].index] = true;
        }
      }
    }
    for (int i = 0; i < n; ++i) {
      if (!alive[i]) continue;
      cout << i << ' ';
    }
    cout << '\n';
  }
}