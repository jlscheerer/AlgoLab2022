#include <bits/stdc++.h>

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

using namespace std;

using InputType = int;
using ExactType = CGAL::Gmpz;

using Program = CGAL::Quadratic_program<InputType>;
using Solution = CGAL::Quadratic_program_solution<ExactType>;

using namespace std;

template <typename T> double to_double(T &obj) {
  return (obj.numerator().to_double() / obj.denominator().to_double());
}

void solve(int n, int d) {
  Program lp(CGAL::SMALLER, false, 0, false, 0);
  const int r = d;
  for (int i = 0; i < n; ++i) {
    int coef = 0;
    for (int j = 0; j < d; ++j) {
      int aij;
      cin >> aij;
      lp.set_a(j, i, aij); //   sum (aij * xj) + ||a|| <= b
      coef += aij * aij;
    }
    coef = (int)sqrt((double)coef);
    lp.set_a(r, i, coef);
    int b;
    cin >> b;
    lp.set_b(i, b);
  }
  lp.set_l(r, true, 0); // r >= 0
  lp.set_c(r, -1);      // maximize r
  Solution s = CGAL::solve_linear_program(lp, ExactType());
  if (s.is_unbounded()) {
    cout << "inf\n";
  } else if (s.is_infeasible()) {
    cout << "none\n";
  } else {
    int obj = floor(-to_double(s.objective_value()));
    cout << obj << "\n";
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  while (true) {
    int n, d;
    cin >> n;
    if (n == 0)
      break;
    cin >> d;
    solve(n, d);
  }
}