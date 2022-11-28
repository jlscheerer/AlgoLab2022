/// 4
#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using traits =
    boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
using graph = boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>;

using vertex_desc = traits::vertex_descriptor;
using edge_desc = traits::edge_descriptor;

using namespace std;

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
    c_map[rev_e] = 0;
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
    vector<int> score(n);
    for (int i = 0; i < n; ++i) {
      cin >> score[i];
    }
    graph G(n);
    edge_adder edges(G);

    const int INF = accumulate(score.begin(), score.end(), 0,
                               [](int x, int y) { return abs(x) + abs(y); });

    for (int i = 0; i < m; ++i) {
      int u, v;
      cin >> u >> v;
      edges.add_edge(u, v, INF);
    }

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    long positives = 0;
    for (int i = 0; i < n; ++i) {
      if (score[i] >= 0) {
        edges.add_edge(v_source, i, score[i]);
        positives += score[i];
      } else {
        edges.add_edge(i, v_sink, -score[i]);
      }
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    long ans = positives - flow;
    if (ans > 0)
      cout << ans << '\n';
    else
      cout << "impossible\n";
  }
}