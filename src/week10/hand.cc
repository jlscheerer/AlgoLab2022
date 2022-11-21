#include <bits/stdc++.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
using Index = int;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

typedef std::tuple<Index, Index, K::FT> Edge;

struct union_find {
public:
  union_find(int n) : m_parent(n), m_size(n, 1) {
    std::iota(m_parent.begin(), m_parent.end(), 0);
  }
  int size() { return m_parent.size(); }
  int find(int x) {
    int par = x;
    while (m_parent[par] != par)
      par = m_parent[par];
    // path compression
    while (x != par) {
      int tmp = m_parent[x];
      m_parent[x] = par;
      x = tmp;
    }
    return par;
  }
  int size_of(int x) { return m_size[find(x)]; }
  bool merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y)
      return false;
    // weighted union
    if (m_size[y] > m_size[x])
      swap(x, y);
    m_parent[y] = x;
    m_size[x] += m_size[y];
    return true;
  }
  set<int> groups() {
    set<int> seen;
    for (int i = 0; i < size(); ++i)
      seen.insert(find(i));
    return seen;
  }

private:
  vector<int> m_parent, m_size;
};

vector<Edge> make_triangulation_edges(const int n, Delaunay &t) {
  vector<Edge> edges;
  edges.reserve(3 * n);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second + 1) % 3)->info();
    Index i2 = e->first->vertex((e->second + 2) % 3)->info();
    if (i1 > i2)
      std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
            [](const Edge &e1, const Edge &e2) -> bool {
              return std::get<2>(e1) < std::get<2>(e2);
            });
  return edges;
}

// Given the size_counts how many families can we construct with at least k
// tents
int divide(unordered_map<int, int> &size_counts, int k) {
  if (k == 1) {
    return size_counts[k];
  } else if (k == 2) { // we can only have size_counts < 2
    return size_counts[k] + size_counts[1] / 2;
  } else if (k == 3) {
    int count = size_counts[k];
    int one_plus_two = min(size_counts[1], size_counts[2]);
    return count + one_plus_two + ((size_counts[1] - one_plus_two) / 3) +
           ((size_counts[2] - one_plus_two) / 2);
  } else if (k == 4) {
    long one_plus_three = min(size_counts[1], size_counts[3]);
    long remainder = max((size_counts[1] - one_plus_three) / 2,
                         (size_counts[3] - one_plus_three));
    return size_counts[k] + one_plus_three + (remainder + size_counts[2]) / 2;
  }
  return -1; // this should never happen! 1 <= k <= 4
}

double find_largest_sq_dist(const int n, vector<Edge> edges, int k, long f0) {
  // keep uniting smallest distance until num_components = f0
  // the smallest remaining diatance is s
  long n_components = n;
  double dist = LONG_MAX;
  union_find uf(n);

  unordered_map<int, int> size_counts;
  size_counts[1] = n;
  for (auto e = edges.begin(); e != edges.end(); ++e) {
    dist = get<2>(*e);
    Index c1 = uf.find(std::get<0>(*e));
    Index c2 = uf.find(std::get<1>(*e));
    if (n_components == f0 && c1 != c2)
      return dist;
    if (c1 != c2) {
      int sc1 = min(uf.size_of(c1), k), sc2 = min(uf.size_of(c2), k);
      --size_counts[sc1], --size_counts[sc2];
      ++size_counts[min(sc1 + sc2, k)];
      if (divide(size_counts, k) < f0)
        break;

      uf.merge(c1, c2);
      if (--n_components == 1)
        break;
    }
  }
  return dist;
}

long find_largest_f_count(const int n, vector<Edge> edges, int k, long s0) {
  // unite all edges with distance < s0
  // number of remaining components is f
  union_find uf(n);
  long n_components = n;

  for (auto e = edges.begin(); e != edges.end(); ++e) {
    double dist = get<2>(*e);
    if (dist >= s0) {
      break; // all edges are now >= s0 (sorted.)
    }
    Index c1 = uf.find(std::get<0>(*e));
    Index c2 = uf.find(std::get<1>(*e));
    if (c1 != c2) {
      uf.merge(c1, c2);
      if (--n_components == 1)
        break;
    }
  }
  unordered_map<int, int> size_counts;
  for (int x : uf.groups()) {
    ++size_counts[min(uf.size_of(x), k)];
  }
  return divide(size_counts, k);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    long n, k, f0, s0;
    cin >> n >> k >> f0 >> s0;
    using IPoint = std::pair<K::Point_2, Index>;
    std::vector<IPoint> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
      long x, y;
      std::cin >> x >> y;
      points.emplace_back(K::Point_2(x, y), i);
    }
    Delaunay triag(points.begin(), points.end());
    vector<Edge> edges = make_triangulation_edges(n, triag);
    long q1 = find_largest_sq_dist(n, edges, k, f0);
    long q2 = find_largest_f_count(n, move(edges), k, s0);
    cout << q1 << ' ' << q2 << '\n';
  }
}