#include <bits/stdc++.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

using IT = int;
using ET = CGAL::Gmpz;

using Program = CGAL::Quadratic_program<IT>;
using Solution = CGAL::Quadratic_program_solution<ET>;

void solve(const int n, const int d, vector<vector<int>> &a, vector<int> &b) {
  vector<int> norms(n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < d; ++j) {
      norms[i] += a[i][j] * a[i][j];
    }
    norms[i] = sqrt(norms[i]);
  }
  
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  // coordinates and radius of the circle
  const auto v_x = [&](int index) {
    return index;
  };
  const int r = d;
  lp.set_l(r, true, 0); // r >= 0
  
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < d; ++j) {
      lp.set_a(v_x(j), i, a[i][j]);
    }
    lp.set_a(r, i, norms[i]);
    lp.set_b(i, b[i]);
  }
  
  // objective function: maximize the radius
  lp.set_c(r, -1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_unbounded()) {
    cout << "inf\n";
  } else if (s.is_infeasible()) {
    cout << "none\n";
  } else {
    auto obj = -s.objective_value();
    long num = obj.numerator().to_double(), den = obj.denominator().to_double();
    cout << num / den << '\n';
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  while (true) {
    int n, d;
    cin >> n;
    if (n == 0) break;
    cin >> d;
    vector<vector<int>> a(n, vector<int>(d));
    vector<int> b(n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < d; ++j) {
        cin >> a[i][j];
      }
      cin >> b[i];
    }
    solve(n, d, a, b);
  }
}