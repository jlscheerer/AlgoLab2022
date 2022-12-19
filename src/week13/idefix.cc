/// 1
#include <bits/stdc++.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

using Point = K::Point_2;
using IPoint = pair<Point, int>;

typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;

int iterate(int n, int m, long s, vector<pair<int, K::FT>> &dists,
            EdgeV &edges) {
  vector<int> bone_counts(n);
  for (int i = 0; i < m; ++i) {
    int index;
    K::FT dist;
    tie(index, dist) = dists[i];
    if (dist > s / 4)
      break;
    ++bone_counts[index];
  }
  boost::disjoint_sets_with_storage<> uf(n);
  for (const auto &edge : edges) {
    Index c1 = uf.find_set(get<0>(edge));
    Index c2 = uf.find_set(get<1>(edge));
    K::FT dist = get<2>(edge);
    if (dist <= s && c1 != c2) {
      uf.link(c1, c2);
    }
  }
  unordered_map<int, int> sums;
  int ans = 0;
  for (int i = 0; i < n; ++i) {
    int set_id = uf.find_set(i);
    sums[set_id] += bone_counts[i];
    ans = max(ans, sums[set_id]);
  }
  return ans;
}

void solve(int n, int m, long s, int k, vector<IPoint> &trees,
           vector<Point> &bones) {
  Delaunay t(trees.begin(), trees.end());
  vector<pair<int, K::FT>> distances;
  distances.reserve(m);
  for (const auto &bone : bones) {
    auto tree = t.nearest_vertex(bone);
    int tree_id = tree->info();
    K::FT dist = CGAL::squared_distance(bone, tree->point());
    distances.emplace_back(tree_id, dist);
  }
  sort(distances.begin(), distances.end(),
       [](const auto &d1, const auto &d2) { return get<1>(d1) < get<1>(d2); });
  EdgeV edges;
  edges.reserve(3 * n);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second + 1) % 3)->info();
    Index i2 = e->first->vertex((e->second + 2) % 3)->info();
    if (i1 > i2)
      swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  sort(edges.begin(), edges.end(), [](const Edge &e1, const Edge &e2) -> bool {
    return get<2>(e1) < get<2>(e2);
  });
  int max_cur = iterate(n, m, s, distances, edges);
  long low = 0, high = 1e15 + 4;
  long target_radius = 0;
  while (low <= high) {
    long middle = low + (high - low) / 2;
    int attempt = iterate(n, m, middle, distances, edges);
    if (attempt >= k) {
      target_radius = middle;
      high = middle - 1;
    } else {
      low = middle + 1;
    }
  }
  cout << max_cur << ' ' << target_radius << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n, m, k;
    long s;
    cin >> n >> m >> s >> k;
    vector<IPoint> trees;
    trees.reserve(n);
    for (int i = 0; i < n; ++i) {
      int x, y;
      cin >> x >> y;
      trees.emplace_back(Point(x, y), i);
    }
    vector<Point> bones;
    bones.reserve(m);
    for (int i = 0; i < m; ++i) {
      int x, y;
      cin >> x >> y;
      bones.emplace_back(x, y);
    }
    solve(n, m, s, k, trees, bones);
  }
}