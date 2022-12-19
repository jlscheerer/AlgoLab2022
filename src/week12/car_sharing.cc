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
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
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

struct Booking {
  int s, t, d, a, p;
};

constexpr int mnT = 0;
constexpr int mxT = 1'000'000;

vector<unordered_map<int, int>> compress_time(int s,
                                              vector<Booking> &bookings) {
  vector<set<int>> relevant_times(s);
  for (int i = 0; i < s; ++i) {
    relevant_times[i].insert(0);
    for (const auto &booking : bookings) {
      relevant_times[i].insert(booking.d);
      relevant_times[i].insert(booking.a);
    }
    relevant_times[i].insert(mxT);
  }

  // TODO determine max size and extend the individual stations
  vector<unordered_map<int, int>> compressed(s);
  for (int i = 0; i < s; ++i) {
    for (const int t : relevant_times[i]) {
      compressed[i][t] = compressed[i].size();
    }
  }
  return compressed;
}

long solve(int n, int s, vector<int> &I, vector<Booking> &bookings) {
  // compress the times (we just care about the order)
  vector<unordered_map<int, int>> compressed = compress_time(s, bookings);
  const int num_times = compressed[0].size();
  auto id = [&](int station, int time) {
    return compressed[station][time] * s + station;
  };
  // cout << mxT << ": " << id(0, 0) << "," << id(0, mxT) << endl;
  graph G(num_times * s);
  edge_adder edges(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  int max_profit = 0, num_cars = 0;
  for (const auto &booking : bookings) {
    max_profit = max(max_profit, booking.p);
  }
  for (int i = 0; i < s; ++i) {
    num_cars += I[i];
  }

  for (int i = 0; i < s; ++i) {
    // add from the source to (s_i, starting_time = 0)
    edges.add_edge(v_source, id(i, mnT), I[i], 0);
    // add edges between successive times in each station
    for (int t = 0; t < mxT; ++t) {
      edges.add_edge(id(i, t), id(i, t + 1), INT_MAX, max_profit);
    }
    // add from (s_i, ending_time = mxT) to the sink
    edges.add_edge(id(i, mxT), v_sink, INT_MAX, 0);
  }

  // add edges for the bookings
  for (const auto &booking : bookings) {
    const int cost =
        (times[booking.a] - times[booking.d]) * max_profit - booking.p;
    edges.add_edge(id(booking.s - 1, booking.d), id(booking.t - 1, booking.a3),
                   1, cost);
  }

  const long expected_cost = num_cars * (times.size() - 1) * max_profit;
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  long cost = boost::find_flow_cost(G);
  return expected_cost - cost;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, s;
    cin >> n >> s;
    vector<int> I(s);
    for (int i = 0; i < s; ++i) {
      cin >> I[i];
    }
    vector<Booking> bookings;
    bookings.reserve(n);
    for (int i = 0; i < n; ++i) {
      int s, t, d, a, p;
      cin >> s >> t >> d >> a >> p;
      bookings.push_back({s, t, d, a, p});
    }
    cout << solve(n, s, I, bookings) << '\n';
  }
}