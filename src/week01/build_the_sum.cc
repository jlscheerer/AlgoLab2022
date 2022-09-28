#include <bits/stdc++.h>

using namespace std;

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n, a = 0, ai;
    cin >> n;
    while (n--) {
      cin >> ai;
      a += ai;
    }
    cout << a << endl;
  }
}