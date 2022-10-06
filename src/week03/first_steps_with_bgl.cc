#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std;

using edge_weight = boost::property<boost::edge_weight_t, int>;

using graph =
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                          boost::no_property, edge_weight>;

template <typename T>
using edge_iterator = typename boost::graph_traits<T>::edge_iterator;

template <typename T>
using edge_descriptor = typename boost::graph_traits<T>::edge_descriptor;

template <typename T>
using vertex_descriptor = typename boost::graph_traits<T>::vertex_descriptor;

template <typename G> int minimum_spanning_tree(const G &g) {
  vector<edge_descriptor<graph>> mst;
  boost::kruskal_minimum_spanning_tree(g, back_inserter(mst));
  auto weights = get(boost::edge_weight, g);
  int mst_weight = 0;
  for (auto it : mst) {
    mst_weight += weights[it];
  }
  return mst_weight;
}

template <typename G> int dijkstra(const G &g, int source) {
  int n = boost::num_vertices(g);
  vector<int> dist_map(n);
  vector<vertex_descriptor<G>> pred_map(n);
  boost::dijkstra_shortest_paths(
      g, source,
      boost::distance_map(
          boost::make_iterator_property_map(dist_map.begin(),
                                            boost::get(boost::vertex_index, g)))
          .predecessor_map(boost::make_iterator_property_map(
              pred_map.begin(), boost::get(boost::vertex_index, g))));
  // find the "furthest" node
  int max_distance = INT_MIN;
  for (int i = 0; i < n; ++i) {
    max_distance = max(max_distance, dist_map[i]);
  }
  return max_distance;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    graph g(n);
    for (int i = 0; i < m; ++i) {
      int u, v, p;
      cin >> u >> v >> p;
      boost::add_edge(u, v, edge_weight(p), g);
    }
    cout << minimum_spanning_tree(g) << " " << dijkstra(g, 0) << endl;
  }
}
