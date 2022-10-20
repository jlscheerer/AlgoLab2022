#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    boost::no_property,
                                    boost::property<boost::edge_weight_t, int>>;
using weight_map = boost::property_map<graph, boost::edge_weight_t>::type;

using namespace std;

int dijkstra_dist(const graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(
      G, s,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));
  return dist_map[t];
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--) {
    int n, m, k, x, y;
    cin >> n >> m >> k >> x >> y;
    const auto id = [&](int node, int river_count) {
      return (k + 1) * node + max(river_count, 0);
    };
    graph G((k + 1) * n);
    weight_map weights = boost::get(boost::edge_weight, G);
    for (int i = 0; i < m; ++i) {
      int a, b, c, d;
      cin >> a >> b >> c >> d;
      for (int r = k; r >= 0; --r) {
        weights[boost::add_edge(id(a, r), id(b, r - d), G).first] = c;
        weights[boost::add_edge(id(b, r), id(a, r - d), G).first] = c;
      }
    }
    cout << dijkstra_dist(G, id(x, k), id(y, 0)) << "\n";
  }
}