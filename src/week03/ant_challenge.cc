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
using edge_descriptor = typename boost::graph_traits<T>::edge_descriptor;

template <typename G>
void update(const G &graph, int source,
            unordered_map<int, unordered_map<int, int>> &min_costs) {
  auto costs = boost::get(boost::edge_weight, graph);
  vector<edge_descriptor<G>> mst;
  boost::kruskal_minimum_spanning_tree(graph, back_inserter(mst));
  for (auto &edge : mst) {
    int u = boost::source(edge, graph);
    int v = boost::target(edge, graph);
    int cost = costs[edge];
    if (v < u)
      swap(u, v);
    if (min_costs[u].count(v)) {
      min_costs[u][v] = min(min_costs[u][v], cost);
    } else
      min_costs[u][v] = cost;
  }
}

template <typename G> int dijkstra(const G &g, int source, int target) {
  int n = boost::num_vertices(g);
  vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(
      g, source,
      boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, g))));
  return dist_map[target];
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n, e, s, a, b;
    cin >> n >> e >> s >> a >> b;
    vector<graph> s_graphs(s, graph(n));
    for (int i = 0; i < e; ++i) {
      int t1, t2;
      cin >> t1 >> t2;
      for (int j = 0; j < s; ++j) {
        int wi;
        cin >> wi;
        boost::add_edge(t1, t2, edge_weight(wi), s_graphs[j]);
      }
    }
    unordered_map<int, unordered_map<int, int>> min_costs;
    for (int i = 0; i < s; ++i) {
      int hive;
      cin >> hive;
      update(s_graphs[i], hive, min_costs);
    }
    graph min_graph(n);
    for (const auto &u_vs : min_costs) {
      int u = u_vs.first;
      for (const auto &v_cost : u_vs.second) {
        int v = v_cost.first, cost = v_cost.second;
        boost::add_edge(u, v, edge_weight(cost), min_graph);
      }
    }
    cout << dijkstra(min_graph, a, b) << endl;
  }
}