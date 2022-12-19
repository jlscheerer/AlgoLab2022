/// 2
#include <bits/stdc++.h>

using namespace std;

constexpr int mxBnd = 16; // == log2 50.000

struct Species {
  Species(int age, int parent) : age(age), parent(parent) {}

  int locate(vector<Species> &species, int age) {
    int ans = 0;
    for (int i = 0; i <= mxBnd; ++i) {
      if (lineage_age[i] <= age) {
        ans = i;
      }
    }
    ans = lineage[ans];
    if (ans == lineage[0])
      return ans;
    return species[ans].locate(species, age);
  }

  int age, parent;
  vector<int> lineage, lineage_age;
};

void create_lineage(int n, vector<Species> &species) {
  for (int i = 0; i < n; ++i) {
    species[i].lineage.push_back(i);
    species[i].lineage_age.push_back(species[i].age);

    species[i].lineage.push_back(species[i].parent);
    species[i].lineage_age.push_back(species[species[i].parent].age);
  }
  for (int it = 1; it <= mxBnd; ++it) {
    for (int i = 0; i < n; ++i) {
      species[i].lineage.push_back(species[species[i].parent].lineage[it]);
      species[i].lineage_age.push_back(
          species[species[i].parent].lineage_age[it]);
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, q;
    cin >> n >> q;
    unordered_map<string, int> ids;
    unordered_map<int, string> names;

    vector<Species> species;
    species.reserve(n);
    for (int i = 0; i < n; ++i) {
      string name;
      int id = i, age;
      cin >> name >> age;
      ids[name] = id;
      names[id] = name;
      species.push_back(Species{age, id});
    }
    for (int i = 0; i < n - 1; ++i) {
      string n1, n2;
      cin >> n1 >> n2;
      int id1 = ids[n1], id2 = ids[n2];
      species[id1].parent = id2;
    }
    create_lineage(n, species);
    for (int i = 0; i < q; ++i) {
      string name;
      int age;
      cin >> name >> age;
      Species &s = species[ids[name]];
      int index = s.locate(species, age);
      cout << names[index] << ' ';
    }
    cout << endl;
  }
}