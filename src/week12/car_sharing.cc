#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

struct Request {
  int s, t, d, a, p;
};

using traits = boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
using graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>>;

using edge_desc = boost::graph_traits<graph>::edge_descriptor;
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
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

long solve(const int n, const int s, vector<int> &l, vector<Request> &requests) {
  // coordinate compression of the order times per station
  vector<map<int, int>> compressed(s);
  const auto insert_compressed = [&](int station, int time) {
    if (compressed[station].find(time) == compressed[station].end()) {
      compressed[station][time] = compressed[station].size();
    }
  };
  int max_profit = 0, max_time = 0;
  for (const auto &req: requests) {
    insert_compressed(req.s, req.d);
    insert_compressed(req.t, req.a);
    max_profit = max(max_profit, req.p);
    max_time = max(max_time, req.a);
  }
  vector<int> prefix(s);
  for (int i = 1; i < s; ++i) {
    prefix[i] = prefix[i - 1] + compressed[i - 1].size();
  }
  graph G(2 + prefix[s - 1] + compressed[s - 1].size());
  edge_adder edges(G);
  const auto c_map = boost::get(boost::edge_capacity, G);
  const auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  const int v_source = 0;
  const int v_sink = 1;
  const auto v_node = [&](int station, int time) {
    return 2 + prefix[station] + compressed[station][time];
  };
  
  // setup the graph for each station
  for (int i = 0; i < s; ++i) {
    if (compressed[i].size() == 0) continue;
    auto it = compressed[i].begin();
    int prev_time = (it++)->first;
    // add the "initial" number of cars to each station
    edges.add_edge(v_source, v_node(i, prev_time), l[i], (prev_time) * max_profit);
    
    // connect sucessive times per station
    for (; it != compressed[i].end(); ++it) {
      int curr_time = it->first;
      edges.add_edge(v_node(i, prev_time), v_node(i, curr_time), INT_MAX, (curr_time - prev_time) * max_profit);
      prev_time = curr_time;
    }
    
    // move the cars at the end to the sink
    edges.add_edge(v_node(i, prev_time), v_sink, INT_MAX, (max_time - prev_time) * max_profit);
  }
  
  // add the requests
  for (const auto &req: requests) {
    edges.add_edge(v_node(req.s, req.d), v_node(req.t, req.a), 1, (req.a - req.d) * max_profit - req.p);
  }
  
  boost::push_relabel_max_flow(G, v_source, v_sink);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  long cost = boost::find_flow_cost(G);
  
  long flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
    flow += c_map[*e] - rc_map[*e];     
  }
  return flow * max_time * max_profit - cost;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, s;
    cin >> n >> s;
    vector<int> l(s);
    for (int i = 0; i < s; ++i) {
      cin >> l[i];
    }
    vector<Request> requests;
    requests.reserve(n);
    for (int i = 0; i < n; ++i) {
      int s, t, d, a, p;
      cin >> s >> t >> d >> a >> p;
      --s, --t; // IMPORTANT: 1 <= s, t
      requests.push_back({s, t, d, a, p});
    }
    cout << solve(n, s, l, requests) << '\n';
  }
}