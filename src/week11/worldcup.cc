/// 3
#include <bits/stdc++.h>

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using InputType = int;
using ExactType = CGAL::Gmpz;
using Program = CGAL::Quadratic_program<InputType>;
using Solution = CGAL::Quadratic_program_solution<ExactType>;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Circle_2 Circle;

using namespace std;

struct Warehouse {
  int x, y, s, a;
};
struct Stadium {
  int x, y, d, u;
};
struct Contour {
  int x, y;
  long r;
};

long round_down(long num, long den) {
  if (num >= 0) {
    return num / den;
  } else {
    return -((-num) / den + ((-num) % den ? 1 : 0));
  }
}

void compute_profit(const int n, const int m, const int c, vector<Warehouse> &w,
                    vector<Stadium> &s, vector<vector<int>> &r) {
  const auto a = [&](int warehouse, int stadium) {
    return warehouse * m + stadium;
  };
  // Ax <= b, lower bound 0 and no upper bounds
  Program lp(CGAL::SMALLER, true, 0, false, 0);
  int equation_id = 0;
  // supply constraints
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      lp.set_a(a(i, j), equation_id, 1);
    }
    lp.set_b(equation_id++, w[i].s);
  }
  // demand constraints
  for (int j = 0; j < m; ++j) {
    // supply to stadium <= demand
    for (int i = 0; i < n; ++i) {
      lp.set_a(a(i, j), equation_id, 1);
    }
    lp.set_b(equation_id++, s[j].d);
    // supply to stadium >= demand
    // <=> - suuply to stadium <= -demand
    for (int i = 0; i < n; ++i) {
      lp.set_a(a(i, j), equation_id, -1);
    }
    lp.set_b(equation_id++, -s[j].d);
  }
  // alcohol percentage constraint
  for (int j = 0; j < m; ++j) {
    for (int i = 0; i < n; ++i) {
      lp.set_a(a(i, j), equation_id, w[i].a);
    }
    lp.set_b(equation_id++, 100 * s[j].u);
  }

  // maximize profit <=> minimize -proft
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      lp.set_c(a(i, j), -r[i][j]);
    }
  }

  Solution solution = CGAL::solve_linear_program(lp, ExactType());
  if (solution.is_infeasible()) {
    cout << "RIOT!\n";
  } else {
    auto obj = -solution.objective_value();
    cout << round_down(obj.numerator().to_double(),
                       100 * obj.denominator().to_double())
         << '\n';
  }
}

void update_costs(const int n, const int m, const int c, vector<Warehouse> &w,
                  vector<Stadium> &s, vector<vector<int>> &r,
                  vector<Contour> &l) {
  vector<Circle> circles;
  circles.reserve(c);
  for (int i = 0; i < c; ++i) {
    circles.emplace_back(Point(l[i].x, l[i].y), K::FT(l[i].r * l[i].r));
  }
  // warehouses
  vector<unordered_set<int>> w_c(n);
  for (int i = 0; i < n; ++i) {
    Point warehouse(w[i].x, w[i].y);
    for (int j = 0; j < c; ++j) {
      if (circles[j].has_on_bounded_side(warehouse)) {
        w_c[i].insert(j);
      }
    }
  }
  // stadiums
  vector<unordered_set<int>> s_c(m);
  for (int i = 0; i < m; ++i) {
    Point stadium(s[i].x, s[i].y);
    for (int j = 0; j < c; ++j) {
      if (circles[j].has_on_bounded_side(stadium)) {
        s_c[i].insert(j);
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      int t = w_c[i].size() + s_c[j].size();
      for (int k : w_c[i]) {
        if (s_c[j].find(k) != s_c[j].end()) {
          t -= 2;
        }
      }
      r[i][j] = 100 * r[i][j] - t;
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, c;
    cin >> n >> m >> c;
    vector<Warehouse> w;
    w.reserve(n);
    for (int i = 0; i < n; ++i) {
      int x, y, s, a;
      cin >> x >> y >> s >> a;
      w.push_back({x, y, s, a});
    }
    vector<Stadium> s;
    s.reserve(m);
    for (int i = 0; i < m; ++i) {
      int x, y, d, u;
      cin >> x >> y >> d >> u;
      s.push_back({x, y, d, u});
    }
    vector<vector<int>> r(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cin >> r[i][j];
      }
    }
    vector<Contour> l;
    l.reserve(c);
    for (int i = 0; i < c; ++i) {
      int x, y, r;
      cin >> x >> y >> r;
      l.push_back({x, y, r});
    }
    update_costs(n, m, c, w, s, r, l);
    compute_profit(n, m, c, w, s, r);
  }
}