#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

using traits =
    boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
using graph = boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<
        boost::edge_capacity_t, long,
        boost::property<
            boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                            boost::property<boost::edge_weight_t, long>>>>>;
using edge_desc = boost::graph_traits<graph>::edge_descriptor;
using vertex_desc = boost::graph_traits<graph>::vertex_descriptor;
using out_edge_it = boost::graph_traits<graph>::out_edge_iterator;

class edge_adder {
  graph &G;

public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;      // new assign cost
    w_map[rev_e] = -cost; // new negative cost
  }
};

using namespace std;

struct Mission {
  int start, end, priority;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, l;
    cin >> n >> m >> l;
    vector<Mission> missions;
    missions.reserve(m);
    int max_priority = 0;
    for (int i = 0; i < m; ++i) {
      int x, y, q;
      cin >> x >> y >> q;
      missions.push_back({x, y, q});
      max_priority = max(max_priority, q);
    }
    graph G(n - 1); // [t0 - t1] -> 0, [t1 -> t2] -> 1
    edge_adder edges(G);

    // Add special vertices source and sink
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    auto station = [&](int end_index) {
      if (end_index == 0)
        return (int)v_sink;
      return end_index - 1;
    };

    // connect source to last train station
    edges.add_edge(v_source, station(n - 1), l, max_priority);
    // connect train stations
    for (int i = n - 1; i > 0; --i) {
      edges.add_edge(station(i), station(i - 1), l, max_priority);
    }

    // connect missions (idea: always prefer taking a missions over "fallback")
    for (int i = 0; i < m; ++i) {
      const auto &mi = missions[i];
      const int m_length = mi.end - mi.start;
      edges.add_edge(station(mi.end), station(mi.start), 1,
                     (m_length * max_priority) - mi.priority);
    }

    boost::push_relabel_max_flow(G, v_source, v_sink);
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);
    cout << n * l * max_priority - cost << '\n';
  }
}