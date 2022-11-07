/// 3
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <bits/stdc++.h>

using namespace std;

using InputType = int;
using ExactType = CGAL::Gmpz;

using Program = CGAL::Quadratic_program<InputType>;
using Solution = CGAL::Quadratic_program_solution<ExactType>;

struct stall {
  int f, c, k, l, m;
};

bool possible(vector<stall> &stalls, int a, int p) {
  const int n = stalls.size();
  // 0 <= hj <= 1
  Program lp(CGAL::LARGER, true, 0, true, 1);

  const int h1 = 0, h2 = 1, h3 = 2;
  for (int i = 0; i < n; ++i) {
    // ki′ = ki +a^2, li′ = li +p^2, mi′ = mi +ap.
    const auto &stall = stalls[i];
    int k = stall.k + a * a;
    int l = stall.l + p * p;
    int m = stall.m + a * p;
    int clean_up = stall.f - stall.c;
    // h1ki + h2li + h3mi >= f_i - c_i
    lp.set_a(h1, i, k);
    lp.set_a(h2, i, l);
    lp.set_a(h3, i, m);
    lp.set_b(i, clean_up);
  }
  Solution s = CGAL::solve_linear_program(lp, ExactType());
  return !s.is_infeasible();
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<stall> stalls;
    stalls.reserve(n);
    for (int i = 0; i < n; ++i) {
      int f, c, k, l, m;
      cin >> f >> c >> k >> l >> m;
      stalls.push_back({f, c, k, l, m});
    }
    vector<int> a(1 + 24), p(1 + 24);
    for (int i = 1; i <= 24; ++i) {
      cin >> a[i];
      a[i] += a[i - 1];
    }
    for (int i = 1; i <= 24; ++i) {
      cin >> p[i];
      p[i] += p[i - 1];
    }
    int bound = 24;
    // binary search the number hours
    int left = 0, right = 2 * bound, ans = -1;

    // for each ai store the max pi that failed so far.
    unordered_map<int, int> cache_a;

    while (left <= right) {
      int hours = (left + right + 1) / 2;
      bool can = false;
      for (int ai = max(0, hours - bound); ai <= min(hours, bound); ++ai) {
        const int pi = hours - ai;

        if (cache_a.find(ai) != cache_a.end() && cache_a[ai] > pi)
          continue;

        if (possible(stalls, a[ai], p[pi])) {
          can = true;
          break;
        } else {
          cache_a[ai] = max(cache_a[ai], pi);
        }
      }
      if (can) {
        ans = hours;
        right = hours - 1;
      } else {
        left = hours + 1;
      }
    }
    if (ans == -1)
      cout << "Impossible!\n";
    else
      cout << ans << '\n';
  }
}