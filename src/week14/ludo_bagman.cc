/// 1
#include <bits/stdc++.h>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<
        boost::edge_capacity_t, long,
        boost::property<
            boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                            boost::property<boost::edge_weight_t, long>>>>>
    graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

// Custom edge adder class
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

struct Match {
  int u, v, r;
};

long solve(int e, int w, int m, int d, int p, int l,
           vector<Match> non_difficult, vector<Match> difficult) {
  graph G(2 + 2 * (e + w) + 2);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  edge_adder edges(G);

  const int v_source = 2 * (e + w);
  const int v_sink = v_source + 1;
  const int v_east = v_sink + 1;
  const int v_west = v_east + 1;

  const auto east = [&](int index, bool non_difficult) {
    return (non_difficult ? 0 : e) + index;
  };
  const auto west = [&](int index, bool non_difficult) {
    return 2 * e + (non_difficult ? 0 : w) + index;
  };

  // connect from the source
  edges.add_edge(v_source, v_east, p - e * l, 0);
  for (int i = 0; i < e; ++i) {
    edges.add_edge(v_source, east(i, true), l, 0);

    edges.add_edge(v_east, east(i, true), INT_MAX / 8, 0);
    edges.add_edge(v_east, east(i, false), INT_MAX / 8, 0);
  }

  // connect to the sink
  edges.add_edge(v_west, v_sink, p - w * l, 0);
  for (int i = 0; i < w; ++i) {
    edges.add_edge(west(i, true), v_sink, l, 0);

    edges.add_edge(west(i, true), v_west, INT_MAX / 8, 0);
    edges.add_edge(west(i, false), v_west, INT_MAX / 8, 0);
  }

  // connect using the matches
  for (const auto &match : non_difficult) {
    edges.add_edge(east(match.u, true), west(match.v, true), 1, match.r);
  }

  for (const auto &match : difficult) {
    edges.add_edge(east(match.u, false), west(match.v, false), 1, match.r);
  }

  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  long flow = 0;
  out_edge_it eit, eend;
  for (boost::tie(eit, eend) = boost::out_edges(boost::vertex(v_source, G), G);
       eit != eend; ++eit) {
    flow += c_map[*eit] - rc_map[*eit];
  }
  if (flow != p)
    return -1;
  return boost::find_flow_cost(G);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int e, w, m, d;
    cin >> e >> w >> m >> d;
    int p, l;
    cin >> p >> l;
    vector<Match> non_difficult;
    non_difficult.reserve(m);
    for (int i = 0; i < m; ++i) {
      int u, v, r;
      cin >> u >> v >> r;
      non_difficult.push_back({u, v, r});
    }
    vector<Match> difficult;
    difficult.reserve(d);
    for (int i = 0; i < d; ++i) {
      int u, v, r;
      cin >> u >> v >> r;
      difficult.push_back({u, v, r});
    }
    long ans = solve(e, w, m, d, p, l, non_difficult, difficult);
    if (ans == -1)
      cout << "No schedule!\n";
    else
      cout << ans << '\n';
  }
}