#include <bits/stdc++.h>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using traits =
    boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
using graph = boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>;
using edge_desc = boost::graph_traits<graph>::edge_descriptor;
using vertex_desc = boost::graph_traits<graph>::vertex_descriptor;
using out_edge_it = boost::graph_traits<graph>::out_edge_iterator;

class edge_adder {
  graph &G;

public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    graph G(n), G_rev(n);
    edge_adder edges(G), edges_rev(G_rev);
    unordered_map<int, unordered_map<int, long>> edge_map;
    for (int i = 0; i < m; ++i) {
      int a, b, c;
      cin >> a >> b >> c;
      edge_map[a][b] += c;
    }
    for (const auto &source_targets : edge_map) {
      int source = source_targets.first;
      for (const auto &target_cost : source_targets.second) {
        int target = target_cost.first;
        long cost = target_cost.second;
        edges.add_edge(source, target, cost);
        edges_rev.add_edge(target, source, cost);
      }
    }
    const int source = 0; // fix any vertex.
    long ans = LONG_MAX;
    for (int end = 0; end < n; ++end) {
      if (end == source)
        continue;
      // we must either take source, or not.
      long flow = boost::push_relabel_max_flow(G, source, end);
      long rev_flow = boost::push_relabel_max_flow(G_rev, source, end);
      ans = min(ans, min(flow, rev_flow));
    }
    cout << ans << endl;
  }
}