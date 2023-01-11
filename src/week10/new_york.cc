#include <bits/stdc++.h>

using namespace std;

struct Window {
  int size() const { return path.size(); }
  int front() const { return path.front().first; }
  
  int abs_diff() const { return max() - min(); }
  int max() const { return *values.rbegin(); }
  int min() const { return *values.begin(); }
  
  void push_front(int index, int h) {
    path.emplace_front(index, h);
    values.insert(h);
  }
  
  int pop_front() {
    int index, h;
    tie(index, h) = path.front();
    path.pop_front();
    values.erase(values.find(h));
    return index;
  }
  
  void push_back(int index, int h) {
    path.emplace_back(index, h);
    values.insert(h);
  }
  
  void pop_back() {
    int index, h;
    tie(index, h) = path.back();
    path.pop_back();
    values.erase(values.find(h));
  }
  
private:
  // [index, value]
  deque<pair<int, int>> path;
  multiset<int> values;
};


void dfs(int n, int m, int k, vector<int> &h, vector<vector<int>> &adj, Window &w, set<int> &ans, int u) {
  // add our self to the end of the path
  w.push_back(u, h[u]);
  
  // save our parent if, we exceed the length
  int par = -1;
  if (w.size() > m)
    par = w.pop_front();

  if(w.size() == m && w.abs_diff() <= k)
    ans.insert(w.front());

  // visit our children
  for(int v : adj[u])
    dfs(n, m, k, h, adj, w, ans, v);
  
  // restore the parent, if removed
  if (par != -1)
    w.push_front(par, h[par]);
  
  // remove ourself again at the end
  w.pop_back();
}

set<int> solve(int n, int m, int k, vector<int> &h, vector<vector<int>> &adj) {
  Window w;
  set<int> ans;
  dfs(n, m, k, h, adj, w, ans, 0);
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while(t--) {
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> h(n);
    for (int i = 0; i < n; ++i) {
      cin >> h[i];
    }
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; ++i) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
    }
    set<int> ans = solve(n, m, k, h, adj);
    if (ans.size() == 0) {
      cout << "Abort mission\n";
      continue;
    }
    for(int x : ans)
      cout << x << ' ';
    cout << '\n';
  }
}