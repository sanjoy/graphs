#pragma once

#include "graph.hpp"
#include "random.hpp"

namespace graph {
std::unique_ptr<Graph> CreateRandomSparseGraph(RandomBitGenerator *gen,
                                               Graph::OrderTy num_vertices,
                                               Graph::OrderTy averge_degree,
                                               bool ensure_connected = true);
}
