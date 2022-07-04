#include "random.hpp"
#include "random_graph.hpp"
#include "test.hpp"

using namespace graph;

static void TestCreateRandomSparseGraph_10_3() {
  std::unique_ptr<RandomBitGenerator> rbg = CreateDefaultRandomBitGenerator();
  std::unique_ptr<Graph> random_graph =
      CreateRandomSparseGraph(rbg.get(), 10, 3, /*ensure_connected=*/false);
  CHECK(!CheckConsistency(random_graph.get()).has_value());
  std::vector<Graph::EdgeType> expected_edges = {
      {1, 0}, {2, 0}, {4, 1}, {5, 1}, {6, 0}, {6, 1}, {6, 5},
      {7, 0}, {7, 2}, {8, 5}, {8, 6}, {9, 1}, {9, 6},
  };

  CHECK_EDGES_EQ(expected_edges, random_graph);
}

static void TestCreateRandomSparseGraph_5_4() {
  std::unique_ptr<RandomBitGenerator> rbg = CreateDefaultRandomBitGenerator();
  std::unique_ptr<Graph> random_graph =
      CreateRandomSparseGraph(rbg.get(), 5, 4, /*ensure_connected=*/false);
  CHECK(!CheckConsistency(random_graph.get()).has_value());

  std::vector<Graph::EdgeType> expected_edges = {
      {1, 0}, {2, 0}, {2, 1}, {3, 0}, {3, 1}, {3, 2}, {4, 1}, {4, 2}, {4, 3},
  };

  CHECK_EDGES_EQ(expected_edges, random_graph);
}

static void TestCreateRandomSparseGraph_10_3_Connected() {
  std::unique_ptr<RandomBitGenerator> rbg = CreateDefaultRandomBitGenerator();
  std::unique_ptr<Graph> random_graph =
      CreateRandomSparseGraph(rbg.get(), 10, 3, /*ensure_connected=*/true);

  std::vector<Graph::EdgeType> expected_edges = {
      {1, 0}, {2, 0}, {4, 1}, {4, 3}, {5, 1}, {6, 0}, {6, 1}, {7, 0},
      {7, 2}, {7, 3}, {7, 5}, {7, 6}, {8, 6}, {9, 1}, {9, 4},
  };

  CHECK_EDGES_EQ(expected_edges, random_graph);
}

#define TEST_LIST(F)                                                           \
  F(TestCreateRandomSparseGraph_10_3)                                          \
  F(TestCreateRandomSparseGraph_5_4)                                           \
  F(TestCreateRandomSparseGraph_10_3_Connected)

DEFINE_MAIN(TEST_LIST)
