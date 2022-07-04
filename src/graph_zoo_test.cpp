#include "graph_viz.hpp"
#include "graph_zoo.hpp"

#include "test.hpp"

#include <set>

using namespace kb;

static void TestCreateCompleteGraph_NoSelfLoops() {
  auto graph = CreateCompleteGraph(10, /*self_loops=*/false);
  Graph::OrderTy vertex_count = graph->GetOrder();

  CHECK(!CheckConsistency(graph.get()).has_value())
  CHECK_EQ(vertex_count, 10);

  std::set<Graph::EdgeTy> edges;
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
  Graph::OrderTy vertex_count = graph->GetOrder();

  CHECK(!CheckConsistency(graph.get()).has_value())
  CHECK_EQ(vertex_count, 10);

  std::set<Graph::EdgeTy> edges;
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
  Graph::OrderTy vertex_count = graph->GetOrder();

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK_EQ(vertex_count, 10);

  for (auto e : Iterate(graph->GetEdges())) {
    (void)e;
    CHECK(false);
  }
}

static void TestCreateCompleteBipartiteGraph_2_5() {
  auto graph = CreateCompleteBipartiteGraph(2, 5);
  Graph::OrderTy vertex_count = graph->GetOrder();

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK_EQ(vertex_count, 7);

  std::set<Graph::EdgeTy> edges;
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
  Graph::OrderTy vertex_count = graph->GetOrder();

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK_EQ(vertex_count, 5);

  for (auto e : Iterate(graph->GetEdges())) {
    (void)e;
    CHECK(false);
  }
}

static void TestCreateCompleteBipartiteGraph_5_0() {
  auto graph = CreateCompleteBipartiteGraph(5, 0);
  Graph::OrderTy vertex_count = graph->GetOrder();

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK_EQ(vertex_count, 5);

  for (auto e : Iterate(graph->GetEdges())) {
    (void)e;
    CHECK(false);
  }
}

static void TestCreateCompleteBipartiteGraph_1_5() {
  auto graph = CreateCompleteBipartiteGraph(1, 5);
  Graph::OrderTy vertex_count = graph->GetOrder();

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK_EQ(vertex_count, 6);

  std::set<Graph::EdgeTy> edges;
  for (auto e : Iterate(graph->GetEdges())) {
    if (e.second == 0)
      std::swap(e.first, e.second);

    CHECK_GE(e.second, 1);
    CHECK_LT(e.second, 6);
    CHECK(edges.insert(e).second);
  }

  CHECK_EQ(edges.size(), 5);
}

static void TestCreateCompleteBipartiteGraph_5_1() {
  auto graph = CreateCompleteBipartiteGraph(5, 1);
  Graph::OrderTy vertex_count = graph->GetOrder();

  CHECK(!CheckConsistency(graph.get()).has_value())

  CHECK_EQ(vertex_count, 6);

  std::set<Graph::EdgeTy> edges;
  for (auto e : Iterate(graph->GetEdges())) {
    if (e.second == 5)
      std::swap(e.first, e.second);

    CHECK_GE(e.second, 0);
    CHECK_LT(e.second, 5);
    CHECK(edges.insert(e).second);
  }

  CHECK_EQ(edges.size(), 5);
}

static void TestReplacementProduct_Ring4_K2() {
  auto outer = CreateRingGraph(4);
  auto inner = CreateCompleteGraph(2, false);

  auto replacement_product =
      CreateReplacementProduct(std::move(outer), std::move(inner));

  std::vector<Graph::EdgeTy> expected_edges = {
      {0, 1}, {0, 2}, {1, 6}, {2, 3}, {3, 4}, {4, 5}, {5, 7}, {6, 7},
  };

  CHECK_EQ(replacement_product->GetOrder(), 8);
  CHECK_EDGES_EQ(expected_edges, replacement_product);
}

static void TestCreateRingGraph_4() {
  auto ring = CreateRingGraph(4);

  std::vector<Graph::EdgeTy> expected_edges = {
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
  };

  CHECK_EQ(ring->GetOrder(), 4);
  CHECK_EDGES_EQ(expected_edges, ring);
}

static void TestCreateRingGraph_2() {
  auto ring = CreateRingGraph(2);

  std::vector<Graph::EdgeTy> expected_edges = {{0, 1}};

  CHECK_EQ(ring->GetOrder(), 2);
  CHECK_EDGES_EQ(expected_edges, ring);
}

#define TEST_LIST(F)                                                           \
  F(TestCreateCompleteGraph_NoSelfLoops)                                       \
  F(TestCreateCompleteGraph_WithSelfLoops)                                     \
  F(TestUnconnectedGraph)                                                      \
  F(TestCreateCompleteBipartiteGraph_2_5)                                      \
  F(TestCreateCompleteBipartiteGraph_0_5)                                      \
  F(TestCreateCompleteBipartiteGraph_5_0)                                      \
  F(TestCreateCompleteBipartiteGraph_1_5)                                      \
  F(TestCreateCompleteBipartiteGraph_5_1)                                      \
  F(TestReplacementProduct_Ring4_K2)                                           \
  F(TestCreateRingGraph_4)                                                     \
  F(TestCreateRingGraph_2)                                                     \
  (void)0;

DEFINE_MAIN(TEST_LIST)
