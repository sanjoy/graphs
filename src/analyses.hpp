#pragma once

#include "graph.hpp"
#include "random.hpp"

namespace graph {
std::optional<Graph::NodeCountType> ComputeMinDegree(Graph *g);
double ComputeCheegerConstantUpperBound(Graph *g, RandomBitGenerator *generator,
                                        int num_iters);
} // namespace graph
