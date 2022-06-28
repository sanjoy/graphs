#include "graph.hpp"

#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace graph {
namespace {
class FiniteNodeIterator : public Graph::NodeIterator {
public:
  FiniteNodeIterator(Graph::NodeCountType num_nodes) : num_nodes_(num_nodes) {}

  Graph::NodeType Get() override { return i_; }

  void Next() override {
    assert(!IsAtEnd());
    i_++;
  }

  bool IsAtEnd() override { return i_ == num_nodes_; }

private:
  Graph::NodeCountType i_ = 0;
  const Graph::NodeCountType num_nodes_;
};

class InfiniteNodeIterator : public Graph::NodeIterator {
public:
  InfiniteNodeIterator() {}

  Graph::NodeType Get() override { return i_; }

  void Next() override { i_++; }

  bool IsAtEnd() override { return false; }

private:
  Graph::NodeCountType i_ = 0;
};
} // namespace

std::unique_ptr<Graph::NodeIterator> Graph::GetNodes() {
  if (auto node_count = GetNodeCountIfFinite())
    return std::make_unique<FiniteNodeIterator>(*node_count);
  return std::make_unique<InfiniteNodeIterator>();
}

namespace {
class ConcreteGraph final : public Graph {
public:
  ConcreteGraph(Graph::NodeCountType num_nodes,
                std::span<Graph::EdgeType> edges)
      : num_nodes_(num_nodes) {
    std::set<Graph::EdgeType> double_edge_set;
    std::set<Graph::EdgeType> single_edge_set;
    for (Graph::EdgeType e : edges) {
      assert(e.first < num_nodes_);
      assert(e.second < num_nodes_);

      double_edge_set.insert(e);
      std::swap(e.first, e.second);
      double_edge_set.insert(e);

      if (e.first < e.second)
        std::swap(e.first, e.second);
      single_edge_set.insert(e);
    }

    for (Graph::EdgeType e : double_edge_set)
      double_edges_.push_back(e);

    for (Graph::EdgeType e : single_edge_set)
      single_edges_.push_back(e);
  }

  class EdgeIterator : public Graph::EdgeIterator {
  public:
    EdgeIterator(std::span<Graph::EdgeType> edges) : edges_(edges) {}

    EdgeType Get() override { return edges_[i_]; }

    void Next() override { i_++; }

    bool IsAtEnd() override { return i_ == edges_.size(); }

  private:
    Graph::NodeCountType i_ = 0;
    std::span<Graph::EdgeType> edges_;
  };

  std::unique_ptr<Graph::EdgeIterator> GetEdges() override {
    return std::make_unique<EdgeIterator>(single_edges_);
  }

  std::optional<NodeCountType> GetNodeCountIfFinite() override {
    return num_nodes_;
  }

  std::unique_ptr<Graph::EdgeIterator>
  GetEdgesWithNode(Graph::NodeType n) override {
    auto it_begin =
        std::upper_bound(double_edges_.begin(), double_edges_.end(), 0,
                         [&](int, Graph::EdgeType e) { return e.first >= n; });
    auto it_end =
        std::upper_bound(double_edges_.begin(), double_edges_.end(), 0,
                         [&](int, Graph::EdgeType e) { return e.first > n; });
    size_t offset = it_begin - double_edges_.begin();
    size_t size = it_end - it_begin;
    return std::make_unique<EdgeIterator>(
        std::span<Graph::EdgeType>(double_edges_).subspan(offset, size));
  }

  std::optional<std::string> CheckConsistency() override {
    return std::nullopt;
  }

private:
  Graph::NodeCountType num_nodes_;
  std::vector<Graph::EdgeType> single_edges_;
  std::vector<Graph::EdgeType> double_edges_;
};
} // namespace

Graph::~Graph() {}

Graph::NodeIterator::~NodeIterator() {}
Graph::EdgeIterator::~EdgeIterator() {}

std::unique_ptr<Graph> CreateConcreteGraph(Graph::NodeCountType num_nodes,
                                           std::span<Graph::EdgeType> edges) {
  return std::make_unique<ConcreteGraph>(num_nodes, edges);
}

std::optional<std::string> CheckConsistency(Graph *g) {
  if (auto r = g->CheckConsistency())
    return r;

  std::set<Graph::EdgeType> double_edges;
  for (Graph::EdgeType e : Iterate(g->GetEdges())) {
    double_edges.insert(e);
    std::swap(e.first, e.second);
    double_edges.insert(e);
  }

  bool ok = true;
  std::stringstream ss;
  for (Graph::NodeType n : Iterate(g->GetNodes())) {
    for (Graph::EdgeType e : Iterate(g->GetEdgesWithNode(n))) {
      auto it = double_edges.find(e);
      if (it == double_edges.end()) {
        ok = false;
        ss << "Edge (" << e.first << ", " << e.second
           << ") found in node adjacency list for " << n
           << " but not in edge list\n";
      } else {
        double_edges.erase(it);
      }
    }
  }

  if (!double_edges.empty()) {
    ok = false;
    ss << "The following edges were found in the edge list but not in any node "
          "adjacency list: ";
    for (auto e : double_edges) {
      ss << "(" << e.first << ", " << e.second << ") ";
    }
    ss << "\n";
  }

  if (ok)
    return std::nullopt;
  else
    return ss.str();
}

std::ostream& operator<<(std::ostream &os, const Graph::EdgeType &e) {
  os << "(" << e.first << ", " << e.second << ")";
  return os;
}
} // namespace graph
