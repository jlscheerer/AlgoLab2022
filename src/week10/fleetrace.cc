#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

using namespace std;

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
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};

struct Pairing {
  int boat_owner, sailor, coefficient;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int b, s, p;
    cin >> b >> s >> p;
    int num_boat_owners = b;
    const auto boat_owner = [&](int index) { return index; };
    const int empty_sailor_id = num_boat_owners;
    int num_sailors = s;
    const auto sailor = [&](int index) { return num_boat_owners + 1 + index; };

    vector<Pairing> pairings;
    pairings.reserve(p);
    int max_coefficient = 0;
    for (int i = 0; i < p; ++i) {
      int bi, si, ci;
      cin >> bi >> si >> ci;
      max_coefficient = max(max_coefficient, ci);
      pairings.push_back({bi, si, ci});
    }

    graph G(num_boat_owners + 1 + num_sailors);
    edge_adder edges(G);

    // Add special vertices source and sink
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for (int i = 0; i < b; ++i) {
      // connect source to boat owners
      edges.add_edge(v_source, boat_owner(i), 1, 0);
      // indicate that we do not want to match this owner with a sailor
      edges.add_edge(boat_owner(i), empty_sailor_id, 1, max_coefficient);
    }

    // add boat_owner <-> sailor edges
    for (const auto &pairing : pairings) {
      edges.add_edge(boat_owner(pairing.boat_owner), sailor(pairing.sailor), 1,
                     max_coefficient - pairing.coefficient);
    }

    for (int i = 0; i < s; ++i) {
      edges.add_edge(sailor(i), v_sink, 1, 0);
    }

    edges.add_edge(empty_sailor_id, v_sink, /*capacity=*/b, 0);

    boost::push_relabel_max_flow(G, v_source, v_sink);
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    long cost = boost::find_flow_cost(G) - b * max_coefficient;

    cout << -cost << endl;
  }
}