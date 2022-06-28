#include "analyses.hpp"
#include "random.hpp"
#include "test.hpp"

#include <vector>

using namespace graph;

static void TestComputeMinDegree_CompleteGraph() {
  std::vector<Graph::EdgeType> edges = {
      {0, 1},
      {0, 2},
      {1, 2},
  };
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  CHECK_HAS_VALUE_AND_ASSIGN(auto min_degree, ComputeMinDegree(graph.get()));
  CHECK_EQ(min_degree, 2);
}

static void TestComputeMinDegree_NullGraph() {
  std::vector<Graph::EdgeType> edges = {};
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  CHECK_HAS_VALUE_AND_ASSIGN(auto min_degree, ComputeMinDegree(graph.get()));
  CHECK_EQ(min_degree, 0);
}

static void TestComputeMinDegree_CompleteGraphWithSelfLoops() {
  std::vector<Graph::EdgeType> edges = {
      {0, 0}, {1, 1}, {2, 2}, {0, 1}, {0, 2}, {1, 2},
  };
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  CHECK_HAS_VALUE_AND_ASSIGN(auto min_degree, ComputeMinDegree(graph.get()));
  CHECK_EQ(min_degree, 3);
}

static void TestComputeExactCheegerConstant_Ring4() {
  std::vector<Graph::EdgeType> edges = {
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
  };
  std::unique_ptr<Graph> ring_graph = CreateConcreteGraph(4, edges);
  CHECK_HAS_VALUE_AND_ASSIGN(double cheeger_constant,
                             ComputeExactCheegerConstant(ring_graph.get()));
  CHECK_EQ(cheeger_constant, 2.0 / 3.0);
}

static std::optional<double>
ComputeExactCheegerConstantForCompleteGraph(int k, bool self_loops) {
  std::vector<Graph::EdgeType> edges;
  for (int i = 0; i < k; i++) {
    if (self_loops)
      edges.push_back({i, i});
    for (int j = i + 1; i < k; i++)
      edges.push_back({i, j});
  }

  std::unique_ptr<Graph> complete_graph = CreateConcreteGraph(k, edges);
  return ComputeExactCheegerConstant(complete_graph.get());
}

static void TestComputeExactCheegerConstant_K20() {
  CHECK_HAS_VALUE_AND_ASSIGN(double cheeger_constant,
                             ComputeExactCheegerConstantForCompleteGraph(
                                 /*k=*/20, /*self_loops=*/false));
  CHECK_EQ(cheeger_constant, 0.052631578947368418);
}

static void TestComputeExactCheegerConstant_K20_WithSelfLoops() {
  CHECK_HAS_VALUE_AND_ASSIGN(double cheeger_constant,
                             ComputeExactCheegerConstantForCompleteGraph(
                                 /*k=*/20, /*self_loops=*/true));
  CHECK_EQ(cheeger_constant, 0.052631578947368418);
}

static void TestComputeExactCheegerConstant_K9() {
  CHECK_HAS_VALUE_AND_ASSIGN(double cheeger_constant,
                             ComputeExactCheegerConstantForCompleteGraph(
                                 /*k=*/20, /*self_loops=*/false));
  CHECK_EQ(cheeger_constant, 0.052631578947368418);
}

static void TestComputeExactCheegerConstant_K9_WithSelfLoops() {
  CHECK_HAS_VALUE_AND_ASSIGN(double cheeger_constant,
                             ComputeExactCheegerConstantForCompleteGraph(
                                 /*k=*/20, /*self_loops=*/true));
  CHECK_EQ(cheeger_constant, 0.052631578947368418);
}

#define TEST_LIST(F)                                                           \
  F(TestComputeMinDegree_CompleteGraph)                                        \
  F(TestComputeMinDegree_NullGraph)                                            \
  F(TestComputeMinDegree_CompleteGraphWithSelfLoops)                           \
  F(TestComputeExactCheegerConstant_Ring4)                                     \
  F(TestComputeExactCheegerConstant_K20)                                       \
  F(TestComputeExactCheegerConstant_K20_WithSelfLoops)                         \
  F(TestComputeExactCheegerConstant_K9)                                        \
  F(TestComputeExactCheegerConstant_K9_WithSelfLoops)

DEFINE_MAIN(TEST_LIST)
