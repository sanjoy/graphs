#include "graph_zoo.hpp"

#include "test.hpp"

#include <set>

using namespace graph;

static void TestCreateCompleteGraph_NoSelfLoops() {
  auto graph = CreateCompleteGraph(10, /*self_loops=*/false);
  Graph::NodeCountType node_count;

  CHECK(!CheckConsistency(graph.get()).has_value())
  CHECK(IsFinite(graph.get(), &node_count));
  CHECK_EQ(node_count, 10);

  std::set<Graph::EdgeType> edges;
  for (auto e : Iterate(graph->GetEdges())) {
    CHECK_LT(e.first, 10);
    CHECK_LT(e.second, 10);
    CHECK_GE(e.first, 0);
    CHECK_GE(e.second, 0);
    CHECK(edges.insert(e).second);
  }

  CHECK_EQ(edges.size(), 45);
}

static void TestCreateCompleteGraph_WithSelfLoops() {
  auto graph = CreateCompleteGraph(10, /*self_loops=*/true);
  Graph::NodeCountType node_count;

  CHECK(!CheckConsistency(graph.get()).has_value())
  CHECK(IsFinite(graph.get(), &node_count));
  CHECK_EQ(node_count, 10);

  std::set<Graph::EdgeType> edges;
  for (auto e : Iterate(graph->GetEdges())) {
    CHECK_LT(e.first, 10);
    CHECK_LT(e.second, 10);
    CHECK_GE(e.first, 0);
    CHECK_GE(e.second, 0);
    CHECK(edges.insert(e).second);
  }

  CHECK_EQ(edges.size(), 55);
}

static void TestUnconnectedGraph() {
  auto graph = CreateUnconnectedGraph(10);
  Graph::NodeCountType node_count;

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK(IsFinite(graph.get(), &node_count));
  CHECK_EQ(node_count, 10);

  for (auto e : Iterate(graph->GetEdges())) {
    (void)e;
    CHECK(false);
  }
}

static void TestCreateCompleteBipartiteGraph_2_5() {
  auto graph = CreateCompleteBipartiteGraph(2, 5);
  Graph::NodeCountType node_count;

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK(IsFinite(graph.get(), &node_count));
  CHECK_EQ(node_count, 7);

  std::set<Graph::EdgeType> edges;
  for (auto e : Iterate(graph->GetEdges())) {
    CHECK_LT(e.first, 7);
    CHECK_GE(e.first, 0);

    CHECK_LT(e.second, 7);
    CHECK_GE(e.second, 0);

    if (e.first < 2)
      CHECK_GE(e.second, 2);
    else
      CHECK_LT(e.second, 2);

    CHECK(edges.insert(e).second);
  }

  CHECK_EQ(edges.size(), 10);
}

static void TestCreateCompleteBipartiteGraph_0_5() {
  auto graph = CreateCompleteBipartiteGraph(0, 5);
  Graph::NodeCountType node_count;

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK(IsFinite(graph.get(), &node_count));
  CHECK_EQ(node_count, 5);

  for (auto e : Iterate(graph->GetEdges())) {
    (void)e;
    CHECK(false);
  }
}

static void TestCreateCompleteBipartiteGraph_5_0() {
  auto graph = CreateCompleteBipartiteGraph(5, 0);
  Graph::NodeCountType node_count;

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK(IsFinite(graph.get(), &node_count));
  CHECK_EQ(node_count, 5);

  for (auto e : Iterate(graph->GetEdges())) {
    (void)e;
    CHECK(false);
  }
}

static void TestCreateCompleteBipartiteGraph_1_5() {
  auto graph = CreateCompleteBipartiteGraph(1, 5);
  Graph::NodeCountType node_count;

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK(IsFinite(graph.get(), &node_count));
  CHECK_EQ(node_count, 6);

  for (auto e : Iterate(graph->GetEdges())) {
    if (e.second == 0)
      std::swap(e.first, e.second);

    CHECK_GE(e.second, 1);
    CHECK_LT(e.second, 6);
  }
}

static void TestCreateCompleteBipartiteGraph_5_1() {
  auto graph = CreateCompleteBipartiteGraph(5, 1);
  Graph::NodeCountType node_count;

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK(IsFinite(graph.get(), &node_count));
  CHECK_EQ(node_count, 6);

  for (auto e : Iterate(graph->GetEdges())) {
    if (e.second == 5)
      std::swap(e.first, e.second);

    CHECK_GE(e.second, 0);
    CHECK_LT(e.second, 5);
  }
}

#define TEST_LIST(F)                                                           \
  F(TestCreateCompleteGraph_NoSelfLoops)                                       \
  F(TestCreateCompleteGraph_WithSelfLoops)                                     \
  F(TestUnconnectedGraph)                                                      \
  F(TestCreateCompleteBipartiteGraph_2_5)                                      \
  F(TestCreateCompleteBipartiteGraph_0_5)                                      \
  F(TestCreateCompleteBipartiteGraph_5_0)                                      \
  F(TestCreateCompleteBipartiteGraph_1_5)                                      \
  F(TestCreateCompleteBipartiteGraph_5_1)

DEFINE_MAIN(TEST_LIST)
