#include "graph_viz.hpp"

#include "graph_zoo.hpp"
#include "test.hpp"

using namespace graph;

static void TestExportToGraphviz_CompleteGraph_NoSelfLoops() {
  auto complete_graph = CreateCompleteGraph(3, /*self_loops=*/false);
  std::string result = ExportToGraphviz(complete_graph.get(), "K3");
  const char *expected = R"(
graph K3 {
  1 -- 0
  2 -- 0
  2 -- 1
}
)";
  CHECK_EQ(result, 1 + expected);
}

#define TEST_LIST(F) F(TestExportToGraphviz_CompleteGraph_NoSelfLoops)

DEFINE_MAIN(TEST_LIST)
