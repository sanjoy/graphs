#pragma once

#include "graph.hpp"

#include <string>

namespace kb {
std::string ExportToGraphviz(Graph *g, std::string name = "G");
bool CreatePngViaGraphviz(Graph *g, std::string filename_base = "/tmp/x",
                          bool open = false);
} // namespace kb
