#include "analyses.hpp"

#include <limits>
#include <vector>

namespace graph {
std::optional<Graph::NodeCountType> ComputeMinDegree(Graph *g) {
  if (!IsFinite(g))
    return std::nullopt;

  Graph::NodeCountType min_degree =
      std::numeric_limits<Graph::NodeCountType>::max();
  for (auto node : Iterate(g->GetNodes())) {
    Graph::NodeCountType degree = 0;
    for (auto edge : Iterate(g->GetEdgesWithNode(node))) {
      (void)edge;
      degree++;
    }
    if (degree < min_degree)
      min_degree = degree;
  }

  return min_degree;
}

namespace {
Graph::NodeCountType PickRandomSubset(RandomBitGenerator *generator,
                                      std::vector<bool> *set) {
  Graph::NodeCountType node_count = 0;
  for (size_t i = 0, e = set->size(); i != e; i++) {
    if (generator->Generate()) {
      node_count++;
      (*set)[i] = true;
    } else {
      (*set)[i] = false;
    }
  }

  return node_count;
}

Graph::NodeCountType FindBoundaryNodes(Graph *g,
                                       const std::vector<bool> &nodes) {
  Graph::NodeCountType boundary_size = 0;
  for (size_t i = 0, e = nodes.size(); i != e; i++) {
    if (nodes[i]) {
      for (auto e : Iterate(g->GetEdgesWithNode(i))) {
        if (!nodes[e.second])
          boundary_size++;
      }
    }
  }
  return boundary_size;
}
} // namespace

double ComputeCheegerConstantUpperBound(Graph *g, RandomBitGenerator *generator,
                                        int num_iters) {
  Graph::NodeCountType node_count;
  if (!IsFinite(g, &node_count))
    return false;

  double upper_bound = std::numeric_limits<double>::infinity();

  std::vector<bool> selected_nodes(node_count);
  for (int i = 0; i < num_iters; i++) {
    Graph::NodeCountType selected_node_count =
        PickRandomSubset(generator, &selected_nodes);

    if (selected_node_count == node_count)
      continue;

    if (selected_node_count > node_count / 2)
      selected_nodes.flip();

    Graph::NodeCountType boundary_nodes = FindBoundaryNodes(g, selected_nodes);
    double this_upper_bound = static_cast<double>(boundary_nodes) /
                              static_cast<double>(selected_node_count);
    upper_bound = std::min(upper_bound, this_upper_bound);
  }

  return upper_bound;
}
} // namespace graph
