#include "graph_analysis.hpp"
#include "random.hpp"
#include "random_graph.hpp"
#include "test.hpp"

#include <vector>

using namespace kb;

static void TestIsRegular_CompleteGraph() {
  std::vector<Graph::EdgeTy> edges = {
      {0, 1},
      {0, 2},
      {1, 2},
  };
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  auto degree = IsRegular(graph.get());
  CHECK(degree.has_value());
  CHECK_EQ(*degree, 2);
}

static void TestIsRegular_NullGraph() {
  std::vector<Graph::EdgeTy> edges = {};
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  auto degree = IsRegular(graph.get());
  CHECK(degree.has_value());
  CHECK_EQ(*degree, 0);
}

static void TestIsRegular_CompleteGraphWithSelfLoops() {
  std::vector<Graph::EdgeTy> edges = {
      {0, 0}, {1, 1}, {2, 2}, {0, 1}, {0, 2}, {1, 2},
  };
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  auto degree = IsRegular(graph.get());
  CHECK(degree.has_value());
  CHECK_EQ(*degree, 3);
}

static void TestIsRegular_IrregularGraph() {
  std::vector<Graph::EdgeTy> edges = {
      {0, 1},
      {1, 2},
  };
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  auto degree = IsRegular(graph.get());
  CHECK(!degree.has_value());
}

static void TestComputeExactCheegerConstant_Ring4() {
  std::vector<Graph::EdgeTy> edges = {
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
  };
  std::unique_ptr<Graph> ring_graph = CreateConcreteGraph(4, edges);
  double cheeger_constant = ComputeExactCheegerConstant(ring_graph.get());
  CHECK_EQ(cheeger_constant, 1.0);
}

static double ComputeExactCheegerConstantForCompleteGraph(int k,
                                                          bool self_loops) {
  std::vector<Graph::EdgeTy> edges;
  for (int i = 0; i < k; i++) {
    if (self_loops)
      edges.push_back({i, i});
    for (int j = i + 1; j < k; j++)
      edges.push_back({i, j});
  }

  std::unique_ptr<Graph> complete_graph = CreateConcreteGraph(k, edges);
  return ComputeExactCheegerConstant(complete_graph.get());
}

static void TestComputeExactCheegerConstant_K20() {
  double cheeger_constant = ComputeExactCheegerConstantForCompleteGraph(
      /*k=*/20, /*self_loops=*/false);
  CHECK_EQ(cheeger_constant, 1.0);
}

static void TestComputeExactCheegerConstant_K20_WithSelfLoops() {
  double cheeger_constant = ComputeExactCheegerConstantForCompleteGraph(
      /*k=*/20, /*self_loops=*/true);
  CHECK_EQ(cheeger_constant, 1.0);
}

static void TestComputeExactCheegerConstant_K9() {
  double cheeger_constant = ComputeExactCheegerConstantForCompleteGraph(
      /*k=*/9, /*self_loops=*/false);
  CHECK_EQ(cheeger_constant, 1.25);
}

static void TestComputeExactCheegerConstant_K9_WithSelfLoops() {
  double cheeger_constant = ComputeExactCheegerConstantForCompleteGraph(
      /*k=*/9, /*self_loops=*/true);
  CHECK_EQ(cheeger_constant, 1.25);
}

static double
ComputeExactCheegerConstantForRandomGraph(Graph::OrderTy order,
                                          Graph::OrderTy avg_degree) {
  auto rbg = CreateDefaultRandomBitGenerator();
  std::unique_ptr<Graph> complete_graph =
      CreateRandomSparseGraph(rbg.get(), order, avg_degree);
  return ComputeExactCheegerConstant(complete_graph.get());
}

static void TestComputeExactCheegerConstant_RandomGraph_9_3() {
  double cheeger_constant = ComputeExactCheegerConstantForRandomGraph(9, 3);
  CHECK_EQ(cheeger_constant, 0.5);
}

static void TestComputeExactCheegerConstant_RandomGraph_10_4() {
  double cheeger_constant = ComputeExactCheegerConstantForRandomGraph(10, 4);
  CHECK_EQ(cheeger_constant, 3.0 / 5.0);
}

static void TestComputeExactCheegerConstant_RandomGraph_15_3() {
  double cheeger_constant = ComputeExactCheegerConstantForRandomGraph(15, 3);
  CHECK_EQ(cheeger_constant, 3.0 / 7.0);
}

static void TestComputeExactCheegerConstant_K10_Disconnected_1() {
  std::vector<Graph::EdgeTy> edges;
  for (int i = 0; i < 9; i++)
    for (int j = i + 1; j < 9; j++)
      edges.push_back({i, j});

  std::unique_ptr<Graph> g = CreateConcreteGraph(10, edges);
  double cheeger_constant = ComputeExactCheegerConstant(g.get());
  CHECK_EQ(cheeger_constant, 0);
}

static void TestComputeExactCheegerConstant_K10_Disconnected_2() {
  std::vector<Graph::EdgeTy> edges;
  for (int i = 0; i < 10; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (i == 5 || j == 5)
        continue;
      edges.push_back({i, j});
    }
  }

  std::unique_ptr<Graph> g = CreateConcreteGraph(10, edges);
  double cheeger_constant = ComputeExactCheegerConstant(g.get());
  CHECK_EQ(cheeger_constant, 0);
}

static void TestComputeExactCheegerConstant_AlmostK10() {
  std::vector<Graph::EdgeTy> edges;
  for (int i = 0; i < 10; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (i == 5 && j == 6)
        continue;

      edges.push_back({i, j});
    }
  }

  std::unique_ptr<Graph> g = CreateConcreteGraph(10, edges);
  double cheeger_constant = ComputeExactCheegerConstant(g.get());
  CHECK_EQ(cheeger_constant, 1.0);
}

#define TEST_LIST(F)                                                           \
  F(TestIsRegular_CompleteGraph)                                               \
  F(TestIsRegular_NullGraph)                                                   \
  F(TestIsRegular_CompleteGraphWithSelfLoops)                                  \
  F(TestIsRegular_IrregularGraph)                                              \
  F(TestComputeExactCheegerConstant_Ring4)                                     \
  F(TestComputeExactCheegerConstant_K20)                                       \
  F(TestComputeExactCheegerConstant_K20_WithSelfLoops)                         \
  F(TestComputeExactCheegerConstant_K9)                                        \
  F(TestComputeExactCheegerConstant_K9_WithSelfLoops)                          \
  F(TestComputeExactCheegerConstant_RandomGraph_9_3)                           \
  F(TestComputeExactCheegerConstant_RandomGraph_10_4)                          \
  F(TestComputeExactCheegerConstant_RandomGraph_15_3)                          \
  F(TestComputeExactCheegerConstant_K10_Disconnected_1)                        \
  F(TestComputeExactCheegerConstant_K10_Disconnected_2)                        \
  F(TestComputeExactCheegerConstant_AlmostK10)                                 \
  (void)0;

DEFINE_MAIN(TEST_LIST)
