#include <bits/stdc++.h>

using namespace std;

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
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

// Custom edge adder class, highly recommended
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

// find all "relevant" edges (i.e., edges contained in a minimum path from s to
// f)
vector<pair<int, int>>
dijkstra(int n, int s, int f, unordered_map<int, vector<pair<int, int>>> &adj) {
  vector<int> dist(n, INT_MAX);
  vector<vector<int>> pred(n);
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
  dist[s] = 0;
  pq.push({0, s});
  while (pq.size()) {
    int d, u;
    tie(d, u) = pq.top();
    pq.pop();
    if (dist[u] < d)
      continue;
    for (const auto &v_d2 : adj[u]) {
      int v, d2;
      tie(v, d2) = v_d2;
      if (dist[v] > d + d2) {
        dist[v] = d + d2;
        pred[v].clear();
        pred[v].push_back(u);
        pq.push({d + d2, v});
      } else if (dist[v] == d + d2) {
        pred[v].push_back(u);
      }
    }
  }
  vector<pair<int, int>> edges;
  set<int> seen;
  deque<int> q{f};
  while (q.size()) {
    int v = q.front();
    q.pop_front();
    if (seen.find(v) != seen.end())
      continue;
    seen.insert(v);
    for (int u : pred[v]) {
      edges.emplace_back(u, v);
      q.push_back(u);
    }
  }
  return edges;
}

long solve(int n, int m, int s, int f,
           unordered_map<int, unordered_map<int, pair<int, int>>> &dist) {
  unordered_map<int, vector<pair<int, int>>> adj;
  for (const auto &u_vs : dist) {
    const int u = u_vs.first;
    for (const auto &v_edg : u_vs.second) {
      int v = v_edg.first, w, l;
      tie(w, l) = v_edg.second;
      adj[u].emplace_back(v, l);
      adj[v].emplace_back(u, l);
    }
  }
  // find all "relevant" edges (i.e., edges contained in a minimum path from s
  // to f)
  vector<pair<int, int>> relevant_edges = dijkstra(n, s, f, adj);
  // now compute the maximum flow from s to f using only these edges...
  graph G(n);
  edge_adder edges(G);
  const int v_source = s, v_sink = f;

  for (const auto &edge : relevant_edges) {
    int u, v, w, l;
    tie(u, v) = edge;
    int x = min(u, v), y = max(u, v);
    tie(w, l) = dist[x][y];
    edges.add_edge(u, v, w);
  }

  return boost::push_relabel_max_flow(G, v_source, v_sink);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, s, f;
    cin >> n >> m >> s >> f;
    unordered_map<int, unordered_map<int, pair<int, int>>> dist;
    for (int i = 0; i < m; ++i) {
      int a, b, w, l;
      cin >> a >> b >> w >> l;
      if (a == b)
        continue; // useless street (will never be in the shortest path!)
      int x = min(a, b), y = max(a, b);
      if (dist[x].find(y) == dist[x].end()) {
        dist[x][y] = {w, l};
      } else {
        int wp, lp;
        tie(wp, lp) = dist[x][y];
        if (l == lp) { // same distance => increases the width
          dist[x][y] = {w + wp, l};
        } else if (l < lp) { // shortest distance => replace prev.
          dist[x][y] = {w, l};
        } // otherwise: useless street (will never be in the shortest path!)
      }
    }
    cout << solve(n, m, s, f, dist) << '\n';
  }
}