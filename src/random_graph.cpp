#include "random_graph.hpp"

#include "logging.hpp"
#include "random.hpp"

#include <vector>

namespace graph {
std::unique_ptr<Graph>
CreateRandomSparseGraph(RandomBitGenerator *gen, Graph::NodeCountType num_nodes,
                        Graph::NodeCountType average_degree,
                        bool ensure_connected) {
  std::vector<Graph::EdgeType> edges;
  for (Graph::NodeCountType i = 0; i < num_nodes; i++) {
    bool some_edge_added = false;
    for (Graph::NodeCountType j = i + 1; j < num_nodes; j++) {
      // The calculation is as follows:
      //
      // Number of edges = Degree * Nodes / 2 = X
      // Number of possible edges = Nodes * (Nodes - 1) / 2 = Y
      // Probability of a specific edge = X / Y = Degree / (Nodes - 1)
      bool should_add_edge =
          GenerateRandomInteger(gen, num_nodes) < average_degree;
      if (should_add_edge) {
        edges.push_back({i, j});
        LOG << "Adding edge " << edges.back() << "\n";
        some_edge_added = true;
      }
    }

    if (!some_edge_added && ensure_connected)
      edges.push_back({i, GenerateRandomInteger(gen, num_nodes)});
  }

  return CreateConcreteGraph(num_nodes, edges);
}
} // namespace graph
