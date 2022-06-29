#include "graph_zoo.hpp"

#include <vector>

namespace graph {
std::unique_ptr<Graph> CreateCompleteGraph(int k, bool self_loops) {
  std::vector<Graph::EdgeType> edges;
  for (int i = 0; i < k; i++) {
    if (self_loops)
      edges.push_back({i, i});
    for (int j = i + 1; j < k; j++)
      edges.push_back({i, j});
  }

  return CreateConcreteGraph(k, edges);
}

std::unique_ptr<Graph> CreateUnconnectedGraph(int k) {
  return CreateConcreteGraph(k, {});
}

std::unique_ptr<Graph> CreateCompleteBipartiteGraph(int l, int r) {
  std::vector<Graph::EdgeType> edges;
  for (int i = 0; i < l; i++)
    for (int j = 0; j < r; j++)
      edges.push_back({i, l + j});
  return CreateConcreteGraph(l + r, edges);
}
} // namespace graph
