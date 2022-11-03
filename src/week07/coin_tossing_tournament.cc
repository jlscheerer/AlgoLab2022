#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

using traits = boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
using graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>;
using vertex_desc = traits::vertex_descriptor;
using edge_desc = traits::edge_descriptor;

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

void solve(int n, int m, vector<int> &s, vector<int> &x, vector<pair<int, int>> &games) {
  bool possible = true;
  int missing = 0;
  // s[i] >= x[i]
  for (int i = 0; i < n; ++i) {
    if (x[i] > s[i]) {
      possible = false;
      break;
    }
    missing += s[i] - x[i];
  }
  possible = possible && missing == (int)games.size();
  if (possible) {
    graph G(games.size() + n);
    edge_adder edges(G);
    for (int i = 0; i < (int)games.size(); ++i) {
      // add edges for each game to each of the possible winners
      edges.add_edge(i, games.size() + games[i].first, 1);
      edges.add_edge(i, games.size() + games[i].second, 1);
    }
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    // add edges from the source to each game
    for (int i = 0; i < (int)games.size(); ++i) {
      edges.add_edge(v_source, i, 1);
    }
    // add edges from each team to the sink
    for (int i = 0; i < n; ++i) {
      edges.add_edge(games.size() + i, v_sink, s[i] - x[i]);
    }
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    possible = flow == (int)games.size(); // all games need to be played.
  }
  
  if (possible) cout << "yes\n";
  else cout << "no\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    vector<int> x(n);
    vector<pair<int, int>> games;
    for (int i = 0; i < m; ++i) {
      int a, b, c;
      cin >> a >> b >> c;
      if (c == 1) {
        ++x[a];
      } else if (c == 2) {
        ++x[b];
      } else if (c == 0) {
        games.emplace_back(a, b);
      }
    }
    vector<int> s(n);
    for (int i = 0; i < n; ++i) {
      cin >> s[i];
    }
    solve(n, m, s, x, games);
  }
}