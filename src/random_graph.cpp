#include "random_graph.hpp"

#include "logging.hpp"
#include "random.hpp"

#include <vector>

namespace kb {
std::unique_ptr<Graph> CreateRandomSparseGraph(RandomBitGenerator *gen,
                                               Graph::OrderTy num_vertices,
                                               Graph::OrderTy average_degree,
                                               bool ensure_connected) {
  std::vector<Graph::EdgeTy> edges;
  for (Graph::OrderTy i = 0; i < num_vertices; i++) {
    bool some_edge_added = false;
    for (Graph::OrderTy j = i + 1; j < num_vertices; j++) {
      // The calculation is as follows:
      //
      // Number of edges = Degree * Vertices / 2 = X
      // Number of possible edges = Vertices * (Vertices - 1) / 2 = Y
      // Probability of a specific edge = X / Y = Degree / (Vertices - 1)
      bool should_add_edge =
          GenerateRandomInteger(gen, num_vertices) < average_degree;
      if (should_add_edge) {
        edges.push_back({i, j});
        LOG << "Adding edge " << edges.back() << "\n";
        some_edge_added = true;
      }
    }

    if (!some_edge_added && ensure_connected)
      edges.push_back({i, GenerateRandomInteger(gen, num_vertices)});
  }

  return CreateConcreteGraph(num_vertices, edges);
}
} // namespace kb
