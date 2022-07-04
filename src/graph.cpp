#include "graph.hpp"

#include "logging.hpp"

#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace graph {
namespace {
class FiniteVertexIterator : public Graph::VertexIterator {
public:
  FiniteVertexIterator(Graph::OrderTy num_vertices)
      : num_vertices_(num_vertices) {}

  Graph::VertexTy Get() override { return i_; }

  void Next() override {
    assert(!IsAtEnd());
    i_++;
  }

  bool IsAtEnd() override { return i_ == num_vertices_; }

private:
  Graph::OrderTy i_ = 0;
  const Graph::OrderTy num_vertices_;
};

class FiniteGraphEdgeIterator final : public Graph::EdgeIterator {
public:
  FiniteGraphEdgeIterator(Graph *g)
      : graph_(g), vertex_iterator_(g->GetVertices()),
        current_edge_iterator_(
            graph_->GetEdgesContainingVertex(vertex_iterator_->Get())) {
    AdvanceToNextValidEdge();
  }

  Graph::EdgeTy Get() override {
    assert(!IsAtEnd());
    return current_edge_iterator_->Get();
  }

  void Next() override {
    assert(!IsAtEnd());

    do {
      current_edge_iterator_->Next();
      AdvanceToNextValidEdge();
    } while (!vertex_iterator_->IsAtEnd() && ShouldSkipCurrentEdge());
  }

  bool IsAtEnd() override { return current_edge_iterator_ == nullptr; }

private:
  bool ShouldSkipCurrentEdge() {
    auto e = current_edge_iterator_->Get();
    if (e.first != vertex_iterator_->Get())
      std::swap(e.first, e.second);

    assert(e.first == vertex_iterator_->Get());
    return e.second < e.first;
  }

  void AdvanceToNextValidEdge() {
    while (current_edge_iterator_->IsAtEnd() && !vertex_iterator_->IsAtEnd()) {
      vertex_iterator_->Next();

      LOG << "Updating current_edge_iterator_\n";
      current_edge_iterator_ =
          graph_->GetEdgesContainingVertex(vertex_iterator_->Get());
    }

    if (vertex_iterator_->IsAtEnd())
      current_edge_iterator_ = nullptr;
  }

  Graph *graph_;
  std::unique_ptr<Graph::VertexIterator> vertex_iterator_;
  std::unique_ptr<Graph::EdgeIterator> current_edge_iterator_;
};
} // namespace

std::unique_ptr<Graph::VertexIterator> Graph::GetVertices() {
  return std::make_unique<FiniteVertexIterator>(GetOrder());
}

std::unique_ptr<Graph::EdgeIterator> Graph::GetEdges() {
  return std::make_unique<FiniteGraphEdgeIterator>(this);
}

std::optional<std::string> Graph::CheckConsistency() { return std::nullopt; }

namespace {
class ConcreteGraph final : public Graph {
public:
  ConcreteGraph(Graph::OrderTy num_vertices, std::span<Graph::EdgeTy> edges)
      : num_vertices_(num_vertices) {
    std::set<Graph::EdgeTy> double_edge_set;
    for (Graph::EdgeTy e : edges) {
      assert(e.first < num_vertices_);
      assert(e.second < num_vertices_);

      double_edge_set.insert(e);
      std::swap(e.first, e.second);
      double_edge_set.insert(e);
    }

    for (Graph::EdgeTy e : double_edge_set)
      edges_.push_back(e);
  }

  class EdgeIterator : public Graph::EdgeIterator {
  public:
    EdgeIterator(std::span<Graph::EdgeTy> edges) : edges_(edges) {}

    EdgeTy Get() override { return edges_[i_]; }

    void Next() override { i_++; }

    bool IsAtEnd() override { return i_ == edges_.size(); }

  private:
    Graph::OrderTy i_ = 0;
    std::span<Graph::EdgeTy> edges_;
  };

  OrderTy GetOrder() override { return num_vertices_; }

  std::unique_ptr<Graph::EdgeIterator>
  GetEdgesContainingVertex(Graph::VertexTy v) override {
    auto it_begin =
        std::upper_bound(edges_.begin(), edges_.end(), 0,
                         [&](int, Graph::EdgeTy e) { return e.first >= v; });
    auto it_end =
        std::upper_bound(edges_.begin(), edges_.end(), 0,
                         [&](int, Graph::EdgeTy e) { return e.first > v; });
    size_t offset = it_begin - edges_.begin();
    size_t size = it_end - it_begin;
    return std::make_unique<EdgeIterator>(
        std::span<Graph::EdgeTy>(edges_).subspan(offset, size));
  }

  std::optional<std::string> CheckConsistency() override {
    return std::nullopt;
  }

private:
  Graph::OrderTy num_vertices_;
  std::vector<Graph::EdgeTy> edges_;
};
} // namespace

Graph::~Graph() {}

Graph::VertexIterator::~VertexIterator() {}
Graph::EdgeIterator::~EdgeIterator() {}

std::unique_ptr<Graph> CreateConcreteGraph(Graph::OrderTy num_vertices,
                                           std::span<Graph::EdgeTy> edges) {
  return std::make_unique<ConcreteGraph>(num_vertices, edges);
}

std::optional<std::string> CheckConsistency(Graph *g) {
  if (auto r = g->CheckConsistency())
    return r;

  std::set<Graph::EdgeTy> double_edges;
  for (Graph::EdgeTy e : Iterate(g->GetEdges())) {
    double_edges.insert(e);
    std::swap(e.first, e.second);
    double_edges.insert(e);
  }

  bool ok = true;
  std::stringstream ss;
  for (Graph::VertexTy v : Iterate(g->GetVertices())) {
    for (Graph::EdgeTy e : Iterate(g->GetEdgesContainingVertex(v))) {
      auto it = double_edges.find(e);
      if (it == double_edges.end()) {
        ok = false;
        ss << "Edge (" << e.first << ", " << e.second
           << ") found in vertex adjacency list for " << v
           << " but not in edge list\n";
      } else {
        double_edges.erase(it);
      }
    }
  }

  if (!double_edges.empty()) {
    ok = false;
    ss << "The following edges were found in the edge list but not in any "
          "vertex "
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

std::ostream &operator<<(std::ostream &os, const Graph::EdgeTy &e) {
  os << "(" << e.first << ", " << e.second << ")";
  return os;
}
} // namespace graph
