#include <bits/stdc++.h>

using namespace std;

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n, x = 0, y;
    cin >> n;
    int even = 0, odd = 0;
    while (n--) {
      cin >> y;
      ++((((x ^= y) & 1) == 0) ? even : odd);
    }
    int even_even = (even * (even - 1)) / 2;
    int odd_odd = (odd * (odd - 1)) / 2;
    cout << (even_even + odd_odd + even) << '\n';
  }
}