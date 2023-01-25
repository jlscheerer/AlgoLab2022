#include <bits/stdc++.h>

using namespace std;

struct Species {
  Species(string &s, int a) : s(s), a(a) {}

  string s;
  int a;
  vector<const Species *> lineage;
};

void build_lineage(int n, vector<Species> &species) {
  constexpr int BOUND = 16; // ceil(log2(50.000))
  for (int k = 1; k < BOUND; ++k) {
    for (int i = 0; i < n; ++i) {
      const Species *par = species[i].lineage[k - 1];
      const Species *next = par->lineage[k - 1];
      species[i].lineage.push_back(next);
    }
  }
}

const Species *find(const Species *s, int b) {
  const Species *ans = nullptr;
  // SHOULD ALWAYS BE TRUE
  if (s->a <= b) {
    ans = s;
  }

  for (const Species *par : s->lineage) {
    if (par->a <= b)
      ans = par;
  }

  if (s == ans)
    return ans;
  return find(ans, b);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, q;
    cin >> n >> q;
    vector<Species> species;
    species.reserve(n);
    unordered_map<string, Species *> map;
    for (int i = 0; i < n; ++i) {
      string s;
      int a;
      cin >> s >> a;
      species.emplace_back(s, a);
      map[s] = &(species[i]);
    }
    for (int i = 0; i < n - 1; ++i) {
      string s, p;
      cin >> s >> p; // is immediate offspring of (s is child of p)
      map[s]->lineage.push_back(map[p]);
    }
    // path the root
    for (int i = 0; i < n; ++i) {
      if (species[i].lineage.size() == 0) {
        species[i].lineage.push_back(&(species[i]));
      }
    }
    build_lineage(n, species);
    for (int i = 0; i < q; ++i) {
      string s;
      int b;
      cin >> s >> b;
      const Species *ans = find(map[s], b);
      cout << ans->s << ' ';
    }
    cout << '\n';
  }
}