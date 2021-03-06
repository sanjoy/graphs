#pragma once

#include "graph.hpp"
#include "random.hpp"

namespace kb {
std::optional<Graph::OrderTy> IsRegular(Graph *g);

// Unclear how to get good probabilistic bounds on the cheeger constant.
double DO_NOT_USE_ComputeCheegerConstantUpperBound(
    Graph *g, RandomBitGenerator *generator, int num_iters);

// Runs in exponential time.
double ComputeExactCheegerConstant(Graph *g);
} // namespace kb
