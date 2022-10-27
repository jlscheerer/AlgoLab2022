#include <bits/stdc++.h>

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

using namespace std;

using InputType = long;
using ExactType = CGAL::Gmpz;

using Program = CGAL::Quadratic_program<InputType>;
using Solution = CGAL::Quadratic_program_solution<ExactType>;

struct Point {
  int x, y;
};
using ll = long long;
using Fraction = CGAL::Quotient<ll>;

template <typename T> ll round_to_nearest_obj(const T &obj) {
  ll num = abs(obj.numerator().to_double());
  ll den = abs(obj.denominator().to_double());
  if (num >= (den + 1) / 2) {
    return abs(obj.numerator().to_double()) / obj.denominator().to_double() +
           (num % den != 0);
  }
  return abs(obj.numerator().to_double()) / obj.denominator().to_double();
}

Fraction make_fraction(ll num, ll den) {
  if (num == 0)
    return {0, 1};
  return {num / __gcd(num, den), den / __gcd(num, den)};
}

Fraction determine_max_scale(const Point &nail, const Point &occ, int w,
                             int h) {
  ll num_limit_x = 2 * abs(nail.x - occ.x) - w;
  ll num_limit_y = 2 * abs(nail.y - occ.y) - h;
  // take the less restrictive constraint.
  if (num_limit_x * h >= num_limit_y * w) {
    return make_fraction(num_limit_x, w);
  }
  return make_fraction(num_limit_y, h);
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    Program lp(CGAL::SMALLER, true, 1, false, 0);
    int n, m, h, w, x, y;
    cin >> n >> m >> h >> w;
    vector<Point> free_nails;
    free_nails.reserve(n);
    for (int i = 0; i < n; ++i) { // free nails
      cin >> x >> y;
      free_nails.push_back({x, y});
    }
    vector<Point> occupied_nails;
    occupied_nails.reserve(m);
    for (int j = 0; j < m; ++j) { // occupied nails
      cin >> x >> y;
      occupied_nails.push_back({x, y});
    }
    int equation_id = 0;
    // bound the free nail scales by the "distance" to the closest occupied
    // nail.
    for (int i = 0; i < n; ++i) {
      const auto &nail = free_nails[i];
      Fraction min_a(1, 0);
      for (int j = 0; j < m; ++j) {
        const auto &occ = occupied_nails[j];
        Fraction scale = determine_max_scale(nail, occ, w, h);
        if (min_a.denominator() == 0 || scale < min_a) {
          min_a = scale;
        }
      }
      if (min_a.denominator() != 0) {
        lp.set_a(i, equation_id, min_a.denominator());
        lp.set_b(equation_id, min_a.numerator());
        ++equation_id;
      }
    }
    // bound the free nail scales using the other free nail scales.
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        ll num_limit_x = 2 * abs(free_nails[i].x - free_nails[j].x);
        ll num_limit_y = 2 * abs(free_nails[i].y - free_nails[j].y);
        // take the less restrictive constraint.
        if (num_limit_x * h >= num_limit_y * w) {
          lp.set_a(i, equation_id, w);
          lp.set_a(j, equation_id, w);
          lp.set_b(equation_id, num_limit_x);
        } else {
          lp.set_a(i, equation_id, h);
          lp.set_a(j, equation_id, h);
          lp.set_b(equation_id, num_limit_y);
        }
        ++equation_id;
      }
    }
    // objective function maximize sum  2(h + w)a_i
    //               i.e. minimize sum -2(h + w)a_i
    for (int i = 0; i < n; ++i) {
      lp.set_c(i, -2 * (h + w));
    }
    Solution s = CGAL::solve_linear_program(lp, ExactType());
    cout << round_to_nearest_obj(s.objective_value()) << "\n";
  }
}