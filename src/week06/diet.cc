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

void solve(int n, int m) {
  Program lp(CGAL::SMALLER, true, 0, false, 0);
  vector<pair<int, int>> bounds;
  bounds.reserve(n);
  for (int i = 0; i < n; ++i) {
    int mini, maxi;
    cin >> mini >> maxi;
    bounds.emplace_back(mini, maxi);
  }
  vector<int> p(m);
  vector<vector<int>> C(n, vector<int>(m));
  for (int j = 0; j < m; ++j) {
    cin >> p[j];
    for (int i = 0; i < n; ++i) {
      cin >> C[i][j];
    }
  }
  // -a1 * Ci,1 - ... - aj * Ci, j <= -mini
  //  a1 * Ci,1 + ... + aj * Ci, j <=  maxi
  for (int i = 0; i < n; ++i) {
    int mini, maxi;
    tie(mini, maxi) = bounds[i];
    for (int j = 0; j < m; ++j) {
      lp.set_a(j, 2 * i, -C[i][j]);
      lp.set_a(j, 2 * i + 1, C[i][j]);
    }
    lp.set_b(2 * i, -mini);
    lp.set_b(2 * i + 1, maxi);
  }
  // objective function min (a1 * p1 + ... + aj * pj)
  for (int j = 0; j < m; ++j) {
    lp.set_c(j, p[j]);
  }
  Solution s = CGAL::solve_linear_program(lp, ExactType());
  if (s.is_infeasible()) {
    cout << "No such diet.\n";
    return;
  }
  cout << (int)floor(to_double(s.objective_value())) << "\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  while (true) {
    int n, m;
    cin >> n >> m;
    if (n == 0 && m == 0)
      break;
    solve(n, m);
  }
}