#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

struct edge_component_t {
  enum { cnum = 555 };
  typedef boost::edge_property_tag kind;
} edge_component;

using component = boost::property<edge_component_t, int>;
using graph =
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                          boost::no_property, component>;

template <typename T>
using vertex_descriptor = typename boost::graph_traits<T>::vertex_descriptor;

template <typename T>
using edge_iterator = typename boost::graph_traits<T>::edge_iterator;

using namespace std;

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    graph g;
    for (int i = 0; i < m; ++i) {
      int e1, e2;
      cin >> e1 >> e2;
      boost::add_edge(e1, e2, g);
    }
    auto components = boost::get(edge_component, g);
    boost::biconnected_components(g, components);
    vector<int> component_counts(n);
    edge_iterator<graph> e_beg, e_end;
    for (boost::tie(e_beg, e_end) = boost::edges(g); e_beg != e_end; ++e_beg) {
      int c = components[*e_beg];
      ++component_counts[c];
    }
    vector<pair<int, int>> critical_edges;
    for (boost::tie(e_beg, e_end) = boost::edges(g); e_beg != e_end; ++e_beg) {
      int c = components[*e_beg];
      if (component_counts[c] == 1) {
        int u = boost::source(*e_beg, g);
        int v = boost::target(*e_beg, g);
        if (v < u)
          std::swap(u, v);
        critical_edges.emplace_back(u, v);
      }
    }
    sort(critical_edges.begin(), critical_edges.end());
    cout << critical_edges.size() << "\n";
    for (const auto &e : critical_edges) {
      cout << e.first << " " << e.second << "\n";
    }
  }
}