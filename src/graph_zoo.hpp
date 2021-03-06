#pragma once

#include "graph.hpp"

#include <memory>

namespace kb {
std::unique_ptr<Graph> CreateCompleteGraph(int k, bool self_loops);

std::unique_ptr<Graph> CreateUnconnectedGraph(int k);

std::unique_ptr<Graph> CreateCompleteBipartiteGraph(int l, int r);

std::unique_ptr<Graph> CreateReplacementProduct(std::unique_ptr<Graph> outer,
                                                std::unique_ptr<Graph> inner);

std::unique_ptr<Graph> CreateRingGraph(int v);
} // namespace kb
