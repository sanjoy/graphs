#include "graph_viz.hpp"

#include <cstdlib>
#include <fstream>
#include <set>
#include <sstream>

namespace graph {
std::string ExportToGraphviz(Graph *g, std::string name) {
  std::stringstream out;

  out << "graph " << name << " {\n";
  std::set<Graph::NodeType> unconnected_nodes;
  for (auto n : Iterate(g->GetNodes()))
    unconnected_nodes.insert(n);

  for (auto e : Iterate(g->GetEdges())) {
    out << "  " << e.first << " -- " << e.second << "\n";
    unconnected_nodes.erase(e.first);
    unconnected_nodes.erase(e.second);
  }

  for (auto n : unconnected_nodes)
    out << "  " << n << "\n";

  out << "}\n";
  return out.str();
}

bool CreatePngViaGraphviz(Graph *g, std::string filename_base, bool open) {
  std::string filename_dot = filename_base + ".dot";
  std::string filename_png = filename_base + ".png";
  {
    std::ofstream dotfile(filename_dot);

    if (!dotfile.is_open())
      return false;

    dotfile << ExportToGraphviz(g);
  }

  std::string dot_cmd =
      std::string("dot < ") + filename_dot + " -Tpng > " + filename_png;
  if (std::system(dot_cmd.c_str()))
    return false;

  if (open) {
    std::string open_cmd = std::string("open ") + filename_png;
    if (std::system(open_cmd.c_str()))
      return false;
  }

  return true;
}
} // namespace graph
