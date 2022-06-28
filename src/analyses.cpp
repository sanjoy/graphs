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

static Graph::NodeCountType PickRandomSubset(RandomBitGenerator *generator,
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

static Graph::NodeCountType FindBoundaryNodes(Graph *g,
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

double DO_NOT_USE_ComputeCheegerConstantUpperBound(
    Graph *g, RandomBitGenerator *generator, int num_iters) {
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

    if (selected_node_count > node_count / 2) {
      selected_nodes.flip();
      selected_node_count = node_count - selected_node_count;
    }

    Graph::NodeCountType boundary_nodes = FindBoundaryNodes(g, selected_nodes);
    double this_upper_bound = static_cast<double>(boundary_nodes) /
                              static_cast<double>(selected_node_count);
    upper_bound = std::min(upper_bound, this_upper_bound);
  }

  return upper_bound;
}

static int IntegerToBits(unsigned long input, std::vector<bool> *output) {
  int set_bits = 0;
  for (int i = 0, e = output->size(); i != e; i++) {
    (*output)[i] = input % 2 == 1;
    if (input % 2 == 1)
      set_bits++;
    input /= 2;
  }
  assert(input == 0 && "output vector is too small!");
  return set_bits;
}

std::optional<double> ComputeExactCheegerConstant(Graph *g) {
  Graph::NodeCountType node_count;
  if (!IsFinite(g, &node_count))
    return std::nullopt;

  int kMaxNodeCount = 24;
  if (node_count > kMaxNodeCount)
    return std::nullopt;

  double upper_bound = std::numeric_limits<double>::infinity();

  int total_combinations = 1u << (node_count - 1);
  std::vector<bool> selected_nodes(node_count);

  for (unsigned i = 0; i != total_combinations; i++) {
    int selected_node_count = IntegerToBits(i, &selected_nodes);
    Graph::NodeCountType boundary_nodes = FindBoundaryNodes(g, selected_nodes);
    double this_upper_bound = static_cast<double>(boundary_nodes) /
                              static_cast<double>(selected_node_count);
    upper_bound = std::min(upper_bound, this_upper_bound);
  }

  return upper_bound;
}
} // namespace graph
