
#include <bits/stdc++.h>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using graph =
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

vector<int> dijkstra(int n, int s,
                     unordered_map<int, vector<pair<int, int>>> &adj,
                     int source, vector<int> &shelters) {
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
  vector<int> dist(n, INT_MAX);
  dist[source] = 0;
  pq.push({0, source});
  while (pq.size()) {
    int d, u;
    tie(d, u) = pq.top();
    pq.pop();
    if (dist[u] < d)
      continue;
    for (const auto v_d2 : adj[u]) {
      int v, d2;
      tie(v, d2) = v_d2;
      if (dist[v] > d + d2) {
        dist[v] = d + d2;
        pq.push({d + d2, v});
      }
    }
  }
  vector<int> ans(s);
  for (int i = 0; i < s; ++i) {
    ans[i] = dist[shelters[i]];
  }
  return ans;
}

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);
  boost::edmonds_maximum_cardinality_matching(
      G, boost::make_iterator_property_map(mate_map.begin(),
                                           boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(
      G, boost::make_iterator_property_map(mate_map.begin(),
                                           boost::get(boost::vertex_index, G)));
  return matching_size;
}

bool attempt(int a, int s, int c, int d, vector<vector<int>> &dist, int t) {
  graph G(a + s + s);
  const auto agent = [&](int x) { return x; };
  const auto station = [&](int x, bool first) {
    return first ? a + x : a + s + x;
  };
  const int time_to_station = t - d;
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < s; ++j) {
      // first agent entering the shelter needs to arrive lastest by t - 2 * d
      if (c == 2 && dist[i][j] <= time_to_station - d) {
        boost::add_edge(agent(i), station(j, true), G);
      }
      if (dist[i][j] <= time_to_station) {
        boost::add_edge(agent(i), station(j, false), G);
      }
    }
  }
  return maximum_matching(G) >= a;
}

int solve(int n, int m, int a, int s, int c, int d,
          unordered_map<int, vector<pair<int, int>>> &adj, vector<int> &agents,
          vector<int> &shelters) {
  vector<vector<int>> dist;
  dist.reserve(a);
  for (int i = 0; i < a; ++i) {
    dist.push_back(dijkstra(n, s, adj, agents[i], shelters));
  }
  int left = 0, right = 1e7, ans = -1;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (attempt(a, s, c, d, dist, mid)) {
      ans = mid;
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, a, s, c, d;
    cin >> n >> m >> a >> s >> c >> d;
    unordered_map<int, vector<pair<int, int>>> adj;
    for (int i = 0; i < m; ++i) {
      char w;
      int x, y, z;
      cin >> w >> x >> y >> z;
      if (w == 'S') {
        adj[x].emplace_back(y, z);
      } else if (w == 'L') {
        adj[x].emplace_back(y, z);
        adj[y].emplace_back(x, z);
      }
    }
    vector<int> agents(a);
    for (int i = 0; i < a; ++i) {
      cin >> agents[i];
    }
    vector<int> shelters(s);
    for (int i = 0; i < s; ++i) {
      cin >> shelters[i];
    }
    cout << solve(n, m, a, s, c, d, adj, agents, shelters) << '\n';
  }
}