#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    boost::no_property,
                                    boost::property<boost::edge_weight_t, int>>;
using weight_map = boost::property_map<graph, boost::edge_weight_t>::type;
using edge_desc = boost::graph_traits<graph>::edge_descriptor;
using vertex_desc = boost::graph_traits<graph>::vertex_descriptor;

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
    edge_desc e;
    for (int i = 0; i < m; ++i) {
      int a, b, c, d;
      cin >> a >> b >> c >> d;
      if (d) {
        // river - decrease river count
        for (int r = k; r >= 0; --r) {
          e = boost::add_edge(id(a, r), id(b, r - 1), G).first;
          weights[e] = c;
          e = boost::add_edge(id(b, r), id(a, r - 1), G).first;
          weights[e] = c;
        }
      } else {
        // no river - keep river count
        for (int r = k; r >= 0; --r) {
          e = boost::add_edge(id(a, r), id(b, r), G).first;
          weights[e] = c;
          e = boost::add_edge(id(b, r), id(a, r), G).first;
          weights[e] = c;
        }
      }
    }
    cout << dijkstra_dist(G, id(x, k), id(y, 0)) << "\n";
  }
}