#include <bits/stdc++.h>

using namespace std;

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

vector<pair<int, int>> dijkstra(int n, int s, int f,
                                vector<unordered_map<int, int>> &ds) {
  vector<int> dist(n, INT_MAX / 2);
  vector<set<int>> pred(n);
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
  pq.push({0, s});
  dist[s] = 0;

  while (pq.size()) {
    int d, u;
    tie(d, u) = pq.top();
    pq.pop();
    if (d > dist[u])
      continue;
    for (const auto &v_d2 : ds[u]) {
      const int v = v_d2.first, d2 = v_d2.second;
      if (dist[v] == d + d2) {
        pred[v].insert(u);
      } else if (dist[v] > d + d2) {
        dist[v] = d + d2;
        pq.push({d + d2, v});
        pred[v].clear();
        pred[v].insert(u);
      }
    }
  }

  deque<int> q;
  q.push_back(f);
  vector<pair<int, int>> ans;
  set<int> seen;
  while (q.size()) {
    int v = q.front();
    q.pop_front();
    if (seen.count(v))
      continue;
    seen.insert(v);
    for (int u : pred[v]) {
      q.push_back(u);
      ans.push_back({u, v});
    }
  }
  return ans;
}

long solve(int n, int m, int s, int f, vector<unordered_map<int, int>> &cs,
           vector<unordered_map<int, int>> &ds) {
  vector<pair<int, int>> adj = dijkstra(n, s, f, ds);
  graph G(n);
  edge_adder edges(G);
  for (const auto &edge : adj) {
    int u, v;
    tie(u, v) = edge;
    int c = cs[u][v];
    edges.add_edge(u, v, c);
  }
  return boost::push_relabel_max_flow(G, s, f);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, s, f;
    cin >> n >> m >> s >> f;
    vector<unordered_map<int, int>> cs(n), ds(n);
    for (int i = 0; i < m; ++i) {
      int a, b, c, d;
      cin >> a >> b >> c >> d;
      if (a > b)
        swap(a, b);
      if (cs[a].find(b) == cs[a].end() || ds[a][b] > d) {
        cs[a][b] = cs[b][a] = c;
        ds[a][b] = ds[b][a] = d;
      } else if (ds[a][b] == d) {
        cs[a][b] += c, cs[b][a] += c;
      }
    }
    cout << solve(n, m, s, f, cs, ds) << '\n';
  }
}