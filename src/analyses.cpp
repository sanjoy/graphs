#include "analyses.hpp"

#include "logging.hpp"

#include <limits>
#include <vector>

namespace graph {
std::optional<Graph::NodeCountType> IsRegular(Graph *g) {
  std::optional<Graph::NodeCountType> degree;
  for (auto node : Iterate(g->GetNodes())) {
    Graph::NodeCountType this_degree = 0;
    for (auto edge : Iterate(g->GetEdgesWithNode(node))) {
      (void)edge;
      this_degree++;
    }

    if (!degree) {
      degree = this_degree;
      continue;
    }

    if (*degree != this_degree)
      return std::nullopt;
  }

  return degree.value_or(0);
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
  std::vector<bool> boundary_set(nodes.size(), false);
  for (size_t i = 0, e = nodes.size(); i != e; i++) {
    if (nodes[i]) {
      for (auto e : Iterate(g->GetEdgesWithNode(i))) {
        if (!nodes[e.second] && !boundary_set[e.second]) {
          boundary_size++;
          boundary_set[e.second] = true;
        }
      }
    }
  }
  return boundary_size;
}

double DO_NOT_USE_ComputeCheegerConstantUpperBound(
    Graph *g, RandomBitGenerator *generator, int num_iters) {
  Graph::NodeCountType node_count = g->GetNodeCount();
  ;

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

std::ostream &operator<<(std::ostream &os, const std::vector<bool> &node_set) {
  os << "{";
  bool first = true;
  for (int i = 0, e = node_set.size(); i != e; i++) {
    if (node_set[i]) {
      if (!first)
        os << ", ";
      first = false;
      os << i;
    }
  }
  os << "}";
  return os;
}

double ComputeExactCheegerConstant(Graph *g) {
  Graph::NodeCountType node_count = g->GetNodeCount();

  double upper_bound = std::numeric_limits<double>::infinity();
  Graph::NodeCountType selected_node_count_for_min = -1;
  Graph::NodeCountType boundary_nodes_for_min = -1;

  int total_combinations = 1u << node_count;
  std::vector<bool> selected_nodes(node_count);

  LOG_VAR(node_count);

  for (unsigned i = 1; i != total_combinations; i++) {
    Graph::NodeCountType selected_node_count =
        IntegerToBits(i, &selected_nodes);
    if (selected_node_count > node_count / 2)
      continue;

    Graph::NodeCountType boundary_nodes = FindBoundaryNodes(g, selected_nodes);
    double this_upper_bound = static_cast<double>(boundary_nodes) /
                              static_cast<double>(selected_node_count);

    assert(boundary_nodes <= (node_count - selected_node_count) &&
           "Cannot have more boundary nodes that the number of nodes outside "
           "the region!");
    LOG_VAR(selected_node_count);
    LOG_VAR(selected_nodes);
    LOG_VAR(boundary_nodes);

    if (this_upper_bound < upper_bound) {
      upper_bound = this_upper_bound;
      selected_node_count_for_min = selected_node_count;
      boundary_nodes_for_min = boundary_nodes;
    }
    upper_bound = std::min(upper_bound, this_upper_bound);
  }

  LOG_VAR(selected_node_count_for_min);
  LOG_VAR(boundary_nodes_for_min);

  return upper_bound;
}
} // namespace graph
