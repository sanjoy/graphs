#include "analyses.hpp"

#include "logging.hpp"

#include <limits>
#include <vector>

namespace graph {
std::optional<Graph::OrderTy> IsRegular(Graph *g) {
  std::optional<Graph::OrderTy> degree;
  for (auto vertex : Iterate(g->GetVertices())) {
    Graph::OrderTy this_degree = 0;
    for (auto edge : Iterate(g->GetEdgesContainingVertex(vertex))) {
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

static Graph::OrderTy PickRandomSubset(RandomBitGenerator *generator,
                                       std::vector<bool> *set) {
  Graph::OrderTy vertex_count = 0;
  for (size_t i = 0, e = set->size(); i != e; i++) {
    if (generator->Generate()) {
      vertex_count++;
      (*set)[i] = true;
    } else {
      (*set)[i] = false;
    }
  }

  return vertex_count;
}

static Graph::OrderTy FindBoundaryVertices(Graph *g,
                                           const std::vector<bool> &vertices) {
  Graph::OrderTy boundary_size = 0;
  std::vector<bool> boundary_set(vertices.size(), false);
  for (size_t i = 0, e = vertices.size(); i != e; i++) {
    if (vertices[i]) {
      for (auto e : Iterate(g->GetEdgesContainingVertex(i))) {
        if (!vertices[e.second] && !boundary_set[e.second]) {
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
  Graph::OrderTy vertex_count = g->GetOrder();
  ;

  double upper_bound = std::numeric_limits<double>::infinity();

  std::vector<bool> selected_vertices(vertex_count);
  for (int i = 0; i < num_iters; i++) {
    Graph::OrderTy selected_vertex_count =
        PickRandomSubset(generator, &selected_vertices);

    if (selected_vertex_count == vertex_count)
      continue;

    if (selected_vertex_count > vertex_count / 2) {
      selected_vertices.flip();
      selected_vertex_count = vertex_count - selected_vertex_count;
    }

    Graph::OrderTy boundary_vertices =
        FindBoundaryVertices(g, selected_vertices);
    double this_upper_bound = static_cast<double>(boundary_vertices) /
                              static_cast<double>(selected_vertex_count);
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

std::ostream &operator<<(std::ostream &os,
                         const std::vector<bool> &vertex_set) {
  os << "{";
  bool first = true;
  for (int i = 0, e = vertex_set.size(); i != e; i++) {
    if (vertex_set[i]) {
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
  Graph::OrderTy vertex_count = g->GetOrder();

  double upper_bound = std::numeric_limits<double>::infinity();
  Graph::OrderTy selected_vertex_count_for_min = -1;
  Graph::OrderTy boundary_vertices_for_min = -1;

  int total_combinations = 1u << vertex_count;
  std::vector<bool> selected_vertices(vertex_count);

  LOG_VAR(vertex_count);

  for (unsigned i = 1; i != total_combinations; i++) {
    Graph::OrderTy selected_vertex_count = IntegerToBits(i, &selected_vertices);
    if (selected_vertex_count > vertex_count / 2)
      continue;

    Graph::OrderTy boundary_vertices =
        FindBoundaryVertices(g, selected_vertices);
    double this_upper_bound = static_cast<double>(boundary_vertices) /
                              static_cast<double>(selected_vertex_count);

    assert(boundary_vertices <= (vertex_count - selected_vertex_count) &&
           "Cannot have more boundary vertices that the number of vertices "
           "outside "
           "the region!");
    LOG_VAR(selected_vertex_count);
    LOG_VAR(selected_vertices);
    LOG_VAR(boundary_vertices);

    if (this_upper_bound < upper_bound) {
      upper_bound = this_upper_bound;
      selected_vertex_count_for_min = selected_vertex_count;
      boundary_vertices_for_min = boundary_vertices;
    }
    upper_bound = std::min(upper_bound, this_upper_bound);
  }

  LOG_VAR(selected_vertex_count_for_min);
  LOG_VAR(boundary_vertices_for_min);

  return upper_bound;
}
} // namespace graph
