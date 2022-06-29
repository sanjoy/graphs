#pragma once

#include "graph.hpp"

#include <memory>

namespace graph {
std::unique_ptr<Graph> CreateCompleteGraph(int k, bool self_loops);

std::unique_ptr<Graph> CreateUnconnectedGraph(int k);

std::unique_ptr<Graph> CreateCompleteBipartiteGraph(int l, int r);
} // namespace graph
