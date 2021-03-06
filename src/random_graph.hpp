#pragma once

#include "graph.hpp"
#include "random.hpp"

namespace kb {
std::unique_ptr<Graph> CreateRandomSparseGraph(RandomBitGenerator *gen,
                                               Graph::OrderTy order,
                                               Graph::OrderTy averge_degree,
                                               bool ensure_connected = true);
}
