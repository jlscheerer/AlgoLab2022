#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using graph =
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
template <typename T>
using vertex_descriptor = typename boost::graph_traits<T>::vertex_descriptor;

using namespace std;

int num_in_common(vector<vector<int>> &students, int i, int j) {
  vector<int> intersect;
  set_intersection(students[i].begin(), students[i].end(), students[j].begin(),
                   students[j].end(), back_inserter(intersect));
  return intersect.size();
}

template <typename G> int maximum_matching(const G &g) {
  int n = boost::num_vertices(g);
  vector<vertex_descriptor<G>> mate_map(n);

  boost::edmonds_maximum_cardinality_matching(
      g, boost::make_iterator_property_map(mate_map.begin(),
                                           boost::get(boost::vertex_index, g)));

  return boost::matching_size(
      g, boost::make_iterator_property_map(mate_map.begin(),
                                           boost::get(boost::vertex_index, g)));
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n, c, f;
    cin >> n >> c >> f;
    unordered_map<string, int> characteristics;
    vector<vector<int>> students(n, vector<int>(c));
    string chara;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < c; ++j) {
        cin >> chara;
        if (!characteristics.count(chara))
          characteristics[chara] = characteristics.size();
        students[i][j] = characteristics[chara];
      }
      sort(students[i].begin(), students[i].end());
    }
    graph g(n);
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        if (num_in_common(students, i, j) > f) {
          boost::add_edge(i, j, g);
        }
      }
    }
    int matching = maximum_matching(g);
    if (matching == n / 2)
      cout << "not optimal\n";
    else
      cout << "optimal\n";
  }
}