#include "graph.hpp"
#include "test.hpp"

#include <iostream>
#include <vector>

using namespace graph;

namespace {
void TestIterators_0() {
  std::vector<Graph::EdgeType> edges = {
      {0, 0}, {0, 2}, {0, 4}, {1, 3}, {3, 1},
  };
  std::unique_ptr<Graph> concrete_graph = CreateConcreteGraph(5, edges);
  CHECK(!CheckConsistency(concrete_graph.get()).has_value());
}

void TestIterators_1() {
  std::vector<Graph::EdgeType> edges = {
      {0, 1},
      {0, 2},
      {1, 2},
  };
  std::unique_ptr<Graph> concrete_graph = CreateConcreteGraph(3, edges);
  CHECK(!CheckConsistency(concrete_graph.get()).has_value());
}

void TestIterators_2() {
  std::vector<Graph::EdgeType> edges = {
      {0, 1}, {0, 2}, {1, 2}, {0, 0}, {1, 1}, {2, 2},
  };
  std::unique_ptr<Graph> concrete_graph = CreateConcreteGraph(3, edges);
  CHECK(!CheckConsistency(concrete_graph.get()).has_value());
}
} // namespace

#define TEST_LIST(F)                                                           \
  F(TestIterators_0)                                                           \
  F(TestIterators_1)                                                           \
  F(TestIterators_2)

int main() { RUN_MAIN(TEST_LIST); }
