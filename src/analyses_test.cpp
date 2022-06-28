#include "analyses.hpp"
#include "random.hpp"
#include "test.hpp"

#include <vector>

using namespace graph;

namespace {
void TestComputeMinDegree_CompleteGraph() {
  std::vector<Graph::EdgeType> edges = {
      {0, 1},
      {0, 2},
      {1, 2},
  };
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  CHECK_HAS_VALUE(auto min_degree, ComputeMinDegree(graph.get()));
  CHECK_EQ(min_degree, 2);
}

void TestComputeMinDegree_NullGraph() {
  std::vector<Graph::EdgeType> edges = {};
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  CHECK_HAS_VALUE(auto min_degree, ComputeMinDegree(graph.get()));
  CHECK_EQ(min_degree, 0);
}

void TestComputeMinDegree_CompleteGraphWithSelfLoops() {
  std::vector<Graph::EdgeType> edges = {
      {0, 0}, {1, 1}, {2, 2}, {0, 1}, {0, 2}, {1, 2},
  };
  std::unique_ptr<Graph> graph = CreateConcreteGraph(3, edges);
  CHECK_HAS_VALUE(auto min_degree, ComputeMinDegree(graph.get()));
  CHECK_EQ(min_degree, 3);
}

void TestComputeCheegerConstantUpperBound_Ring_Iter1() {
  std::vector<Graph::EdgeType> edges = {
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
  };
  std::unique_ptr<Graph> ring_graph = CreateConcreteGraph(4, edges);
  std::unique_ptr<RandomBitGenerator> bit_gen =
      CreateDefaultRandomBitGenerator(1);
  double cheeger_upper_bound =
      ComputeCheegerConstantUpperBound(ring_graph.get(), bit_gen.get(), 1);
  CHECK_EQ(cheeger_upper_bound, 2.0 / 3.0);
}

void TestComputeCheegerConstantUpperBound_Ring_Iter5() {
  std::vector<Graph::EdgeType> edges = {
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
  };
  std::unique_ptr<Graph> ring_graph = CreateConcreteGraph(4, edges);
  std::unique_ptr<RandomBitGenerator> bit_gen =
      CreateDefaultRandomBitGenerator(1);
  double cheeger_upper_bound =
      ComputeCheegerConstantUpperBound(ring_graph.get(), bit_gen.get(), 5);
  CHECK_EQ(cheeger_upper_bound, 2.0 / 3.0);
}

void TestComputeCheegerConstantUpperBound_K20_Iter5() {
  std::vector<Graph::EdgeType> edges;
  for (int i = 0; i < 20; i++)
    for (int j = i + 1; i < 20; i++)
      edges.push_back({i, j});

  std::unique_ptr<Graph> ring_graph = CreateConcreteGraph(20, edges);
  std::unique_ptr<RandomBitGenerator> bit_gen =
      CreateDefaultRandomBitGenerator(1);
  double cheeger_upper_bound =
      ComputeCheegerConstantUpperBound(ring_graph.get(), bit_gen.get(), 5);
  CHECK_EQ(cheeger_upper_bound, 1.0);
}
} // namespace

#define TEST_LIST(F)                                                           \
  F(TestComputeMinDegree_CompleteGraph)                                        \
  F(TestComputeMinDegree_NullGraph)                                            \
  F(TestComputeMinDegree_CompleteGraphWithSelfLoops)                           \
  F(TestComputeCheegerConstantUpperBound_Ring_Iter1)                           \
  F(TestComputeCheegerConstantUpperBound_Ring_Iter5)                           \
  F(TestComputeCheegerConstantUpperBound_K20_Iter5)

DEFINE_MAIN(TEST_LIST)
