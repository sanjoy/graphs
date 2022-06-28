#pragma once

#include "graph.hpp"
#include "random.hpp"

namespace graph {
std::unique_ptr<Graph>
CreateRandomSparseGraph(RandomBitGenerator *gen, Graph::NodeCountType num_nodes,
                        Graph::NodeCountType averge_degree,
                        bool ensure_connected = true);
}
