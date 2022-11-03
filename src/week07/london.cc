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

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int h, w;
    cin >> h >> w;
    string note;
    cin >> note;
    const int n = note.size();
    vector<vector<char>> front(h, vector<char>(w)), back(h, vector<char>(w));
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        cin >> front[i][j];
      }
    }
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        cin >> back[i][j];
      }
    }
    // group the same snippets (only 26^2 / 2 cases!)
    const int letters = 'Z' - 'A' + 1;
    vector<vector<int>> snippets(letters, vector<int>(letters));
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        char front_c = front[i][j] - 'A', back_c = back[i][w - j - 1] - 'A';
        ++snippets[min(front_c, back_c)][max(front_c, back_c)];
      }
    }
    const int num_snippets = 26 * 26;

    graph G(num_snippets + letters);
    edge_adder edges(G);
    // connect snippets to the letters
    for (int i = 0; i < letters; ++i) {
      for (int j = i; j < letters; ++j) {
        int count = snippets[i][j];
        if (count == 0)
          continue;
        edges.add_edge(i * letters + j, num_snippets + i, count);
        if (i != j) {
          edges.add_edge(i * letters + j, num_snippets + j, count);
        }
      }
    }
    const vertex_desc v_source = boost::add_vertex(G);
    // add from the source to all of the snippets
    for (int i = 0; i < letters; ++i) {
      for (int j = i; j < letters; ++j) {
        int count = snippets[i][j];
        if (count == 0)
          continue;
        edges.add_edge(v_source, i * letters + j, count);
      }
    }
    unordered_map<char, int> counts;
    for (char c : note) {
      ++counts[c];
    }
    // add from the letters to the sink
    const vertex_desc v_sink = boost::add_vertex(G);
    for (int i = 0; i < letters; ++i) {
      edges.add_edge(num_snippets + i, v_sink, counts[i + 'A']);
    }
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    bool possible = flow == n;
    if (possible)
      cout << "Yes\n";
    else
      cout << "No\n";
  }
}