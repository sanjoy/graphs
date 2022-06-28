#include "random.hpp"
#include "random_graph.hpp"
#include "test.hpp"

using namespace graph;

static void TestCreateRandomSparseGraph_10_3() {
  std::unique_ptr<RandomBitGenerator> rbg = CreateDefaultRandomBitGenerator();
  std::unique_ptr<Graph> random_graph =
      CreateRandomSparseGraph(rbg.get(), 10, 3, /*ensure_connected=*/false);
  CHECK(!CheckConsistency(random_graph.get()).has_value());

  std::vector<Graph::EdgeType> edges;
  for (auto e : Iterate(random_graph->GetEdges()))
    edges.push_back(e);

  CHECK_EQ(edges.size(), 13);

  CHECK_EQ(edges[0], Graph::EdgeType(1, 0));
  CHECK_EQ(edges[1], Graph::EdgeType(2, 0));
  CHECK_EQ(edges[2], Graph::EdgeType(4, 1));
  CHECK_EQ(edges[3], Graph::EdgeType(5, 1));
  CHECK_EQ(edges[4], Graph::EdgeType(6, 0));
  CHECK_EQ(edges[5], Graph::EdgeType(6, 1));
  CHECK_EQ(edges[6], Graph::EdgeType(6, 5));
  CHECK_EQ(edges[7], Graph::EdgeType(7, 0));
  CHECK_EQ(edges[8], Graph::EdgeType(7, 2));
  CHECK_EQ(edges[9], Graph::EdgeType(8, 5));
  CHECK_EQ(edges[10], Graph::EdgeType(8, 6));
  CHECK_EQ(edges[11], Graph::EdgeType(9, 1));
  CHECK_EQ(edges[12], Graph::EdgeType(9, 6));
}

static void TestCreateRandomSparseGraph_5_4() {
  std::unique_ptr<RandomBitGenerator> rbg = CreateDefaultRandomBitGenerator();
  std::unique_ptr<Graph> random_graph =
      CreateRandomSparseGraph(rbg.get(), 5, 4, /*ensure_connected=*/false);
  CHECK(!CheckConsistency(random_graph.get()).has_value());

  std::vector<Graph::EdgeType> edges;
  for (auto e : Iterate(random_graph->GetEdges()))
    edges.push_back(e);

  CHECK_EQ(edges.size(), 9);

  CHECK_EQ(edges[0], Graph::EdgeType(1, 0));
  CHECK_EQ(edges[1], Graph::EdgeType(2, 0));
  CHECK_EQ(edges[2], Graph::EdgeType(2, 1));
  CHECK_EQ(edges[3], Graph::EdgeType(3, 0));
  CHECK_EQ(edges[4], Graph::EdgeType(3, 1));
  CHECK_EQ(edges[5], Graph::EdgeType(3, 2));
  CHECK_EQ(edges[6], Graph::EdgeType(4, 1));
  CHECK_EQ(edges[7], Graph::EdgeType(4, 2));
  CHECK_EQ(edges[8], Graph::EdgeType(4, 3));
}

static void TestCreateRandomSparseGraph_10_3_Connected() {
  std::unique_ptr<RandomBitGenerator> rbg = CreateDefaultRandomBitGenerator();
  std::unique_ptr<Graph> random_graph =
      CreateRandomSparseGraph(rbg.get(), 10, 3, /*ensure_connected=*/true);

  std::vector<Graph::EdgeType> edges;
  for (auto e : Iterate(random_graph->GetEdges()))
    edges.push_back(e);

  CHECK_EQ(edges.size(), 15);

  CHECK_EQ(edges[0], Graph::EdgeType(1, 0));
  CHECK_EQ(edges[1], Graph::EdgeType(2, 0));
  CHECK_EQ(edges[2], Graph::EdgeType(4, 1));
  CHECK_EQ(edges[3], Graph::EdgeType(4, 3));
  CHECK_EQ(edges[4], Graph::EdgeType(5, 1));
  CHECK_EQ(edges[5], Graph::EdgeType(6, 0));
  CHECK_EQ(edges[6], Graph::EdgeType(6, 1));
  CHECK_EQ(edges[7], Graph::EdgeType(7, 0));
  CHECK_EQ(edges[8], Graph::EdgeType(7, 2));
  CHECK_EQ(edges[9], Graph::EdgeType(7, 3));
  CHECK_EQ(edges[10], Graph::EdgeType(7, 5));
  CHECK_EQ(edges[11], Graph::EdgeType(7, 6));
  CHECK_EQ(edges[12], Graph::EdgeType(8, 6));
  CHECK_EQ(edges[13], Graph::EdgeType(9, 1));
  CHECK_EQ(edges[14], Graph::EdgeType(9, 4));
}

#define TEST_LIST(F)                                                           \
  F(TestCreateRandomSparseGraph_10_3)                                          \
  F(TestCreateRandomSparseGraph_5_4)                                           \
  F(TestCreateRandomSparseGraph_10_3_Connected)

DEFINE_MAIN(TEST_LIST)
