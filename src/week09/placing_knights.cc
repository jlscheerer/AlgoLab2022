#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using traits =
    boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
using graph = boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>;
using edge_desc = boost::graph_traits<graph>::edge_descriptor;
using vertex_desc = boost::graph_traits<graph>::vertex_descriptor;
using out_edge_it = boost::graph_traits<graph>::out_edge_iterator;

class edge_adder {
  graph &G;

public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

using namespace std;

vector<bool> maximum_independent_set(graph &G, const vertex_desc &v_source) {
  const int N = boost::num_vertices(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  vector<bool> vis(N, false);
  queue<int> Q;
  vis[v_source] = true;
  Q.push(v_source);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend;
         ++ebeg) {
      const int v = boost::target(*ebeg, G);
      if (rc_map[*ebeg] == 0 || vis[v])
        continue;
      vis[v] = true;
      Q.push(v);
    }
  }
  return vis;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<vector<bool>> grid(n, vector<bool>(n));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        int value;
        cin >> value;
        grid[i][j] = value > 0;
      }
    }
    auto index = [&](int i, int j, bool need_valid) {
      if (i < 0 || j < 0 || i >= n || j >= n)
        return -1;
      if (need_valid && !grid[i][j])
        return -1;
      return i * n + j;
    };
    graph G(n * n);
    edge_adder edges(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    vector<pair<int, int>> offsets = {{-1, -2}, {-1, +2}, {+1, -2}, {+1, +2},
                                      {-2, -1}, {-2, +1}, {+2, -1}, {+2, +1}};
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        int id = index(i, j, true);
        if (id == -1)
          continue;
        if ((i + j) % 2 == 0) {
          edges.add_edge(v_source, id, 1);
        } else {
          edges.add_edge(id, v_sink, 1);
        }
        for (const auto &offset : offsets) {
          int other_id = index(i + offset.first, j + offset.second, true);
          if (other_id == -1)
            continue;
          if (id > other_id)
            continue;
          if ((i + j) % 2 == 0) {
            edges.add_edge(id, other_id, 1);
          } else {
            edges.add_edge(other_id, id, 1);
          }
        }
      }
    }

    boost::push_relabel_max_flow(G, v_source, v_sink);
    vector<bool> vis = maximum_independent_set(G, v_source);
    int ans = 0, missing = 0;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        int id = index(i, j, true);
        if (id == -1) {
          ++missing;
          continue;
        }
        if ((i + j) % 2 == 0) {
          ans += vis[id];
        } else {
          ans += !vis[id];
        }
      }
    }
    cout << ans << endl;
  }
}