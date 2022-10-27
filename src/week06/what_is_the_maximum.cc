#include <bits/stdc++.h>

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

using InputType = int;
using ExactType = CGAL::Gmpz;

using Program = CGAL::Quadratic_program<InputType>;
using Solution = CGAL::Quadratic_program_solution<ExactType>;

using namespace std;

template <typename T> double to_double(T &obj) {
  return (obj.numerator().to_double() / obj.denominator().to_double());
}

void solve(int p, int a, int b) {
  const int x = 0;
  const int y = 1;
  const int z = 2;
  if (p == 1) {
    Program lp(CGAL::SMALLER, true, 0, false, 0);
    lp.set_a(x, 0, 1);
    lp.set_a(y, 0, 1);
    lp.set_b(0, 4); // x + y <= 4
    lp.set_a(x, 1, 4);
    lp.set_a(y, 1, 2);
    lp.set_b(1, a * b); // 4x + 2y <= ab
    lp.set_a(x, 2, -1);
    lp.set_a(y, 2, 1);
    lp.set_b(2, 1); // -x + y <= 1

    // objective function (max b * y - a * x) [i.e., min -b * y + a * x]
    lp.set_c(y, -b);
    lp.set_c(x, a);
    Solution s = CGAL::solve_linear_program(lp, ExactType());
    if (s.is_infeasible()) {
      cout << "no\n";
    } else if (s.is_unbounded()) {
      cout << "unbounded\n";
    } else {
      auto obj = (int)floor(-to_double(s.objective_value()));
      cout << obj << "\n";
    }
  } else {
    Program lp(CGAL::LARGER, false, 0, true, 0);
    lp.set_a(x, 0, 1);
    lp.set_a(y, 0, 1);
    lp.set_b(0, -4);
    lp.set_a(x, 1, 4);
    lp.set_a(y, 1, 2);
    lp.set_a(z, 1, 1);
    lp.set_b(1, -a * b);
    lp.set_a(x, 2, -1);
    lp.set_a(y, 2, 1);
    lp.set_b(2, -1);

    // objective function min ax + by + z
    lp.set_c(x, a);
    lp.set_c(y, b);
    lp.set_c(z, 1);

    Solution s = CGAL::solve_linear_program(lp, ExactType());
    if (s.is_infeasible()) {
      cout << "no\n";
    } else if (s.is_unbounded()) {
      cout << "unbounded\n";
    } else {
      auto obj = (int)ceil(to_double(s.objective_value()));
      cout << obj << "\n";
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  while (true) {
    int p, a, b;
    cin >> p;
    if (p == 0)
      break;
    cin >> a >> b;
    solve(p, a, b);
  }
}