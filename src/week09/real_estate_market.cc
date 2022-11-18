#include <bits/stdc++.h>

using namespace std;

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

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, s;
    cin >> n >> m >> s;
    vector<int> limits;
    limits.reserve(s);
    for (int i = 0; i < s; ++i) {
      int limit;
      cin >> limit;
      limits.push_back(limit);
    }
    vector<int> states;
    states.reserve(m);
    for (int i = 0; i < m; ++i) {
      int state;
      cin >> state;
      states.push_back(state - 1);
    }
    vector<vector<int>> bids(n, vector<int>(m));
    int max_bid = 0;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cin >> bids[i][j];
        max_bid = max(max_bid, bids[i][j]);
      }
    }
    const bool INPUT = true, OUTPUT = false;
    const int num_buyers = n;
    auto buyer = [&](int index) { return index; };
    const int num_sites = 2 * m; // input + output
    auto property = [&](int index, bool input) {
      int id = num_buyers + index;
      if (input == OUTPUT)
        id += m;
      return id;
    };
    const int num_states = 2 * s; // input + output
    auto state = [&](int index, bool input) {
      int id = num_buyers + num_sites + index;
      if (input == OUTPUT)
        id += s;
      return id;
    };
    graph G(num_buyers + num_sites + num_states);
    edge_adder edges(G);

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    // connect source to buyers
    for (int i = 0; i < n; ++i) {
      edges.add_edge(v_source, buyer(i), 1, 0);
    }

    // connnect buyers to properties
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        edges.add_edge(buyer(i), property(j, INPUT), 1, max_bid - bids[i][j]);
      }
    }

    // connect properties (input & output)
    for (int i = 0; i < m; ++i) {
      edges.add_edge(property(i, INPUT), property(i, OUTPUT), 1, 0);
    }

    // connect properties to states
    for (int i = 0; i < m; ++i) {
      edges.add_edge(property(i, OUTPUT), state(states[i], INPUT), 1, 0);
    }

    // connect states (input & output)
    for (int i = 0; i < s; ++i) {
      edges.add_edge(state(i, INPUT), state(i, OUTPUT), limits[i], 0);
    }

    // connect states to sink
    for (int i = 0; i < s; ++i) {
      edges.add_edge(state(i, OUTPUT), v_sink, INT_MAX / 8, 0);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);
    // Iterate over all edges leaving the source to sum up the flow values.
    int s_flow = 0;
    out_edge_it e, eend;
    for (boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source, G), G);
         e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];
    }
    cost -= s_flow * max_bid;
    cout << s_flow << ' ' << -cost << '\n';
  }
}