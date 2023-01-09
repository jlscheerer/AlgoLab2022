#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

using weighted_graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int>>;
using weight_map = boost::property_map<weighted_graph, boost::edge_weight_t>::type;
using edge_desc = boost::graph_traits<weighted_graph>::edge_descriptor;
using vertex_desc = boost::graph_traits<weighted_graph>::vertex_descriptor;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  return dist_map[t];
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, k, x, y;
    cin >> n >> m >> k >> x >> y;
    weighted_graph G(n * (k + 1));
    weight_map weights = boost::get(boost::edge_weight, G);
    const auto v_id = [&](int index, int rc) {
      return rc * n + index;
    };
    for (int i = 0; i < m; ++i) {
      int a, b, c, d;
      cin >> a >> b >> c >> d;
      for (int kk = 0; kk <= k; ++kk) {
        edge_desc e1 = boost::add_edge(v_id(a, kk), v_id(b, max(kk - d, 0)), G).first;
        edge_desc e2 = boost::add_edge(v_id(b, kk), v_id(a, max(kk - d, 0)), G).first;
        weights[e1] = c, weights[e2] = c;
      }
    }
    cout << dijkstra_dist(G, v_id(x, k), v_id(y, 0)) << '\n';
  }
}