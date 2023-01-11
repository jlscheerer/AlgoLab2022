#include <bits/stdc++.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

using IT = CGAL::Gmpq;
using ET = CGAL::Gmpq;
using Program = CGAL::Quadratic_program<IT>;
using Solution = CGAL::Quadratic_program_solution<ET>;

struct Nail {
  long x, y;
  
  // bound between two free nails
  CGAL::Gmpq bound(long h, long w, const Nail &other) {
    long x_diff = abs(x - other.x);
    long y_diff = abs(y - other.y);
    return max(CGAL::Gmpq(2 * x_diff, w), CGAL::Gmpq(2 * y_diff, h));
  }
  
  // bound between a free nail and a fixed nail
  CGAL::Gmpq fixed_bound(long h, long w, const Nail &fixed_nail) {
    long x_diff = abs(x - fixed_nail.x);
    long y_diff = abs(y - fixed_nail.y);
    return max(CGAL::Gmpq(2 * x_diff - w, w), CGAL::Gmpq(2 * y_diff - h, h));
  }
  
};

long solve(int n, int m, int h, int w, vector<Nail> &unbounded, vector<Nail> &bounded) {
  // create an LP with Ax <= b, lower bound 1 and no upper bounds
  Program lp (CGAL::SMALLER, true, 1, false, 0); 
  const auto v_a = [&](int index) {
    return index;
  };
  int equation_id = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      lp.set_a(v_a(i), equation_id, 1);
      lp.set_a(v_a(j), equation_id, 1);
      lp.set_b(equation_id++, unbounded[i].bound(h, w, unbounded[j]));
    }
  }
  
  for (int i = 0; i < n; ++i) {
    CGAL::Gmpq upper_bound(LONG_MAX, 1l);
    for (int j = 0; j < m; ++j) {
      upper_bound = min(upper_bound, unbounded[i].fixed_bound(h, w, bounded[j]));
    }
    lp.set_u(v_a(i), true, upper_bound);
  }
  
  // objective function: maximize perimiter
  for (int i = 0; i < n; ++i) {
    lp.set_c(v_a(i), -2 * (h + w));
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  auto obj = -s.objective_value();
  auto ans = obj.numerator() / obj.denominator();
  
  long num = ans.numerator().to_double();
  long den = ans.denominator().to_double();
  
  return (num + den - 1) / den;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, h, w;
    cin >> n >> m >> h >> w;
    vector<Nail> unbounded; // free nails
    unbounded.reserve(n);
    for (int i = 0; i < n; ++i) {
      long x, y;
      cin >> x >> y;
      unbounded.push_back({x, y});
    }
    vector<Nail> bounded; // occupied nails
    bounded.reserve(m);
    for (int i = 0; i < m; ++i) {
      long x, y;
      cin >> x >> y;
      bounded.push_back({x, y});
    }
    cout << solve(n, m, h, w, unbounded, bounded) << '\n';
  }
}