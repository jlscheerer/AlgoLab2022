/// 2
#include <bits/stdc++.h>

using namespace std;

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

using Point = K::Point_2;

#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Asteroid {
  Asteroid(int x, int y, int density) : point(x, y), density(density) {}
  Point point;
  int density;
};

bool solve(const int a, const int s, const int b, const long e,
           vector<Asteroid> &asteroids, vector<Point> &shooting_points,
           vector<Point> &hunters) {
  Triangulation t(hunters.begin(), hunters.end());
  // create an LP with Ax >= b, lower bound 0 and no upper bounds
  Program lp(CGAL::LARGER, true, 0, false, 0);
  for (int j = 0; j < a; ++j) {
    const auto &asteroid = asteroids[j];
    int equation_id = j;
    for (int i = 0; i < s; ++i) {
      K::FT di = CGAL::squared_distance(asteroid.point, shooting_points[i]);
      if (b > 0) {
        K::FT dbound = CGAL::squared_distance(
            shooting_points[i], t.nearest_vertex(shooting_points[i])->point());
        if (di >= dbound)
          continue; // too far away.
      }
      lp.set_a(i, equation_id, CGAL::Gmpq(1, max(K::FT(1), di)));
    }
    // IMPORTANT: set the bound here. To make the LP infeasible correctly.
    lp.set_b(equation_id, asteroid.density);
  }
  // sum over all powers needs to be bound by e
  const int bound_eq_id = a;
  for (int i = 0; i < s; ++i) {
    lp.set_a(i, bound_eq_id, -1);
  }
  lp.set_b(bound_eq_id, -e);
  Solution solution = CGAL::solve_linear_program(lp, ET());
  return !solution.is_infeasible();
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int a, s, b;
    cin >> a >> s >> b;
    long e;
    cin >> e;
    vector<Asteroid> asteroids;
    asteroids.reserve(a);
    for (int i = 0; i < a; ++i) {
      int x, y, d;
      cin >> x >> y >> d;
      asteroids.emplace_back(x, y, d);
    }
    vector<Point> shooting_points;
    shooting_points.reserve(s);
    for (int i = 0; i < s; ++i) {
      int x, y;
      cin >> x >> y;
      shooting_points.emplace_back(x, y);
    }
    vector<Point> hunters;
    hunters.reserve(b);
    for (int i = 0; i < b; ++i) {
      int x, y;
      cin >> x >> y;
      hunters.emplace_back(x, y);
    }
    bool ans = solve(a, s, b, e, asteroids, shooting_points, hunters);
    if (ans)
      cout << "y\n";
    else
      cout << "n\n";
  }
}