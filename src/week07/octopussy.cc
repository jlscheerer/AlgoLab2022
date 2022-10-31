#include <bits/stdc++.h>

using namespace std;

bool recsolve(int index, vector<int> &B, vector<bool> &deactivate,
              vector<int> &pi) {
  const int n = deactivate.size();
  if (index >= n)
    return true;
  if (deactivate[index])
    return true;
  bool ok = true;
  ok = ok && recsolve(index * 2 + 1, B, deactivate, pi);
  ok = ok && recsolve(index * 2 + 2, B, deactivate, pi);
  int bi = B[index];
  ok = ok && (bi > (int)pi.size());
  pi.push_back(index);
  deactivate[index] = true;
  return ok;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> B(n);
    for (int i = 0; i < n; ++i) {
      cin >> B[i];
    }
    vector<bool> deactivate(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < n; ++i) {
      pq.push({B[i], i});
    }
    vector<int> pi;
    pi.reserve(n);
    bool ok = true;
    while (pq.size()) {
      int index = pq.top().second;
      pq.pop();
      if (deactivate[index])
        continue;
      if (!recsolve(index, B, deactivate, pi)) {
        ok = false;
        break;
      }
    }
    if (ok)
      cout << "yes\n";
    else
      cout << "no\n";
  }
}