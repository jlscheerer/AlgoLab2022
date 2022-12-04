/// 1
#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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

struct Debt {
  int i, j, d;
};

bool solve(const int n, const int m, vector<int> &b, vector<Debt> &debts) {
  graph G(n);
  edge_adder edges(G);

  for (const auto &debt : debts) {
    edges.add_edge(debt.i, debt.j, debt.d);
  }

  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  long sum = 0;
  for (int i = 0; i < n; ++i) {
    if (b[i] >= 0) {
      edges.add_edge(v_source, i, b[i]);
      sum += b[i];
    } else {
      edges.add_edge(i, v_sink, -b[i]);
    }
  }
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  return sum > flow;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    vector<int> b(n);
    for (int i = 0; i < n; ++i) {
      cin >> b[i];
    }
    vector<Debt> debts;
    debts.reserve(m);
    for (int k = 0; k < m; ++k) {
      int i, j, d;
      cin >> i >> j >> d;
      debts.push_back({i, j, d});
    }
    bool ans = solve(n, m, b, debts);
    if (ans) {
      cout << "yes\n";
    } else
      cout << "no\n";
  }
}