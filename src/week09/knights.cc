#include <bits/stdc++.h>

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

using namespace std;

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

struct knight {
  int x, y;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int m, n, k, c;
    cin >> m >> n >> k >> c;
    vector<knight> knights;
    knights.reserve(k);
    for (int i = 0; i < k; ++i) {
      int x, y;
      cin >> x >> y;
      knights.push_back({x, y});
    }

    // * 2 (input and output of intersections)
    const int num_intersections = m * n;
    const auto id_ix = [&](int x, int y, bool input) {
      int id = y * m + x;
      return (input ? 0 : num_intersections) + id;
    };

    constexpr bool HORIZONTAL = true, VERTICAL = false;
    constexpr bool INPUT = true, OUTPUT = false;

    const int num_vertical_segments = m * (n + 1);
    const int num_horizontal_segments = n * (m + 1);
    const int num_segments = num_vertical_segments + num_horizontal_segments;
    const auto id_seg_raw = [&](int x, int y, bool direction) {
      if (direction == VERTICAL) {
        int id = x * (n + 1) + y;
        return 2 * num_intersections + id;
      } else {
        int id = y * (m + 1) + x;
        return 2 * num_intersections + num_vertical_segments + id;
      }
    };
    const auto id_seg = [&](int x, int y, bool horizontal, bool input) {
      int id = id_seg_raw(x, y, horizontal);
      return (input ? 0 : num_segments) + id;
    };
    graph G(2 * num_intersections + 2 * num_segments);
    edge_adder edges(G);

    // connect input and output intersections
    for (int x = 0; x < m; ++x) {
      for (int y = 0; y < n; ++y) {
        edges.add_edge(id_ix(x, y, INPUT), id_ix(x, y, OUTPUT), c);
      }
    }

    // connect intersections with the segments
    for (int x = 0; x < m; ++x) {
      for (int y = 0; y < n; ++y) {
        const int intersection_id = id_ix(x, y, OUTPUT);
        edges.add_edge(intersection_id, id_seg(x, y, VERTICAL, INPUT), 1);
        edges.add_edge(intersection_id, id_seg(x, y + 1, VERTICAL, INPUT), 1);

        edges.add_edge(intersection_id, id_seg(x, y, HORIZONTAL, INPUT), 1);
        edges.add_edge(intersection_id, id_seg(x + 1, y, HORIZONTAL, INPUT), 1);
      }
    }

    // Add special vertices source and sink
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for (int x = 0; x < m; ++x) {
      for (int y = 0; y < n + 1; ++y) {
        edges.add_edge(id_seg(x, y, VERTICAL, INPUT),
                       id_seg(x, y, VERTICAL, OUTPUT), 1);

        if (y != 0) {
          edges.add_edge(id_seg(x, y, VERTICAL, OUTPUT), id_ix(x, y - 1, INPUT),
                         1);
        } else {
          edges.add_edge(id_seg(x, y, VERTICAL, OUTPUT), v_sink, 1);
        }

        if (y != n) {
          edges.add_edge(id_seg(x, y, VERTICAL, OUTPUT), id_ix(x, y, INPUT), 1);
        } else {
          edges.add_edge(id_seg(x, y, VERTICAL, OUTPUT), v_sink, 1);
        }
      }
    }

    for (int x = 0; x < m + 1; ++x) {
      for (int y = 0; y < n; ++y) {
        edges.add_edge(id_seg(x, y, HORIZONTAL, INPUT),
                       id_seg(x, y, HORIZONTAL, OUTPUT), 1);

        if (x != 0) {
          edges.add_edge(id_seg(x, y, HORIZONTAL, OUTPUT),
                         id_ix(x - 1, y, INPUT), 1);
        } else {
          edges.add_edge(id_seg(x, y, HORIZONTAL, OUTPUT), v_sink, 1);
        }

        if (x != m) {
          edges.add_edge(id_seg(x, y, HORIZONTAL, OUTPUT), id_ix(x, y, INPUT),
                         1);
        } else {
          edges.add_edge(id_seg(x, y, HORIZONTAL, OUTPUT), v_sink, 1);
        }
      }
    }

    // Add the Knights to the Board
    for (auto &kp : knights) {
      edges.add_edge(v_source, id_ix(kp.x, kp.y, INPUT), 1);
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << flow << '\n';
  }
}