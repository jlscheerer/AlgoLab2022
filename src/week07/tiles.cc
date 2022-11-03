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

void solve(int h, int w, vector<vector<bool>> &grid) {
  graph G(h * w);
  edge_adder edges(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  // split the board into positions with (i + j) % 2 == 0 and (i + j) % 2 == 1
  // connect the two if they can form a domino (with capacity 1).
  int free_fields = 0;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (grid[i][j])
        continue; // no need to place here
      ++free_fields;
      int index = i * w + j;
      if ((i + j) % 2 == 0) {
        // connect with the source
        edges.add_edge(v_source, index, 1);
        // & connect with potential counter parts
        if (i > 0 && !grid[i - 1][j])
          edges.add_edge(index, (i - 1) * w + (j), 1);
        if (i < h - 1 && !grid[i + 1][j])
          edges.add_edge(index, (i + 1) * w + (j), 1);
        if (j > 0 && !grid[i][j - 1])
          edges.add_edge(index, (i)*w + (j - 1), 1);
        if (j < w - 1 && !grid[i][j + 1])
          edges.add_edge(index, (i)*w + (j + 1), 1);
      } else {
        // connect with the sink
        edges.add_edge(index, v_sink, 1);
      }
    }
  }
  bool possible = free_fields % 2 == 0;
  if (possible) {
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    possible = flow == free_fields / 2;
  }
  if (possible)
    cout << "yes\n";
  else
    cout << "no\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int w, h;
    cin >> w >> h;
    vector<vector<bool>> grid(h, vector<bool>(w));
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        char grid_position;
        cin >> grid_position;
        grid[i][j] = grid_position == 'x';
      }
    }
    solve(h, w, grid);
  }
}