#include "graph_viz.hpp"

#include <cstdlib>
#include <fstream>
#include <set>
#include <sstream>

namespace graph {
std::string ExportToGraphviz(Graph *g, std::string name) {
  std::stringstream out;

  out << "graph " << name << " {\n";
  std::set<Graph::VertexTy> unconnected_vertices;
  for (auto v : Iterate(g->GetVertices()))
    unconnected_vertices.insert(v);

  for (auto e : Iterate(g->GetEdges())) {
    out << "  " << e.first << " -- " << e.second << "\n";
    unconnected_vertices.erase(e.first);
    unconnected_vertices.erase(e.second);
  }

  for (auto v : unconnected_vertices)
    out << "  " << v << "\n";

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
