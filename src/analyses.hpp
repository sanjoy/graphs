#pragma once

#include "graph.hpp"
#include "random.hpp"

namespace graph {
std::optional<Graph::NodeCountType> ComputeMinDegree(Graph *g);

// Unclear how to get good probabilistic bounds on the cheeger constant.
double DO_NOT_USE_ComputeCheegerConstantUpperBound(
    Graph *g, RandomBitGenerator *generator, int num_iters);

// Runs in exponential time.
std::optional<double> ComputeExactCheegerConstant(Graph *g);
} // namespace graph
