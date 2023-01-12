#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

using traits =
    boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
using graph = boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>;

class edge_adder {
  graph &G;

public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

struct Knight {
  int x, y;
};

int solve(int m, int n, int k, int c, vector<Knight> &knights) {
  if (m == 0 || n == 0) {
    return 0;
  }

  graph G(2 * m * n);
  edge_adder edges(G);
  const int v_source = boost::add_vertex(G);
  const int v_sink = boost::add_vertex(G);
  const auto v_id = [&](int y, int x, bool output) {
    return y * m + x + (output ? n * m : 0);
  };

  // connect input and output
  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < m; ++x) {
      edges.add_edge(v_id(y, x, false), v_id(y, x, true), c);
    }
  }

  // connect vertical segments
  for (int y = 0; y < n - 1; ++y) {
    for (int x = 0; x < m; ++x) {
      edges.add_edge(v_id(y, x, true), v_id(y + 1, x, false), 1); // forward
      edges.add_edge(v_id(y + 1, x, true), v_id(y, x, false),
                     1); // backward
    }
  }

  // connect horizontal segments
  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < m - 1; ++x) {
      edges.add_edge(v_id(y, x, true), v_id(y, x + 1, false), 1); // forward
      edges.add_edge(v_id(y, x + 1, true), v_id(y, x, false),
                     1); // backward
    }
  }

  // connect output segments (horizontal)
  for (int y = 0; y < n; ++y) {
    edges.add_edge(v_id(y, 0, true), v_sink, 1);
    edges.add_edge(v_id(y, m - 1, true), v_sink, 1);
  }

  // connect output segments (vertical)
  for (int x = 0; x < m; ++x) {
    edges.add_edge(v_id(0, x, true), v_sink, 1);
    edges.add_edge(v_id(n - 1, x, true), v_sink, 1);
  }

  // connect the knights
  for (const auto &knight : knights) {
    edges.add_edge(v_source, v_id(knight.y, knight.x, false), 1);
  }

  return boost::push_relabel_max_flow(G, v_source, v_sink);
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    int m, n, k, c;
    cin >> m >> n >> k >> c;
    vector<Knight> knights;
    knights.reserve(k);
    for (int i = 0; i < k; ++i) {
      int x, y;
      cin >> x >> y;
      knights.push_back({x, y});
    }
    cout << solve(m, n, k, c, knights) << '\n';
  }
}