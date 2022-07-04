#include "graph_zoo.hpp"

#include "analyses.hpp"
#include "logging.hpp"

#include <vector>

namespace graph {
std::unique_ptr<Graph> CreateCompleteGraph(int k, bool self_loops) {
  std::vector<Graph::EdgeType> edges;
  for (int i = 0; i < k; i++) {
    if (self_loops)
      edges.push_back({i, i});
    for (int j = i + 1; j < k; j++)
      edges.push_back({i, j});
  }

  return CreateConcreteGraph(k, edges);
}

std::unique_ptr<Graph> CreateUnconnectedGraph(int k) {
  return CreateConcreteGraph(k, {});
}

std::unique_ptr<Graph> CreateCompleteBipartiteGraph(int l, int r) {
  std::vector<Graph::EdgeType> edges;
  for (int i = 0; i < l; i++)
    for (int j = 0; j < r; j++)
      edges.push_back({i, l + j});
  return CreateConcreteGraph(l + r, edges);
}

namespace {
class ReplacementProduct final : public Graph {
public:
  class ReplacementProductEdgeIterator final : public EdgeIterator {
  public:
    ReplacementProductEdgeIterator(ReplacementProduct *parent, NodeType node)
        : parent_(parent) {
      std::tie(outer_node_, inner_node_) = parent_->Split(node);
      inner_node_edge_iterator_ =
          parent_->GetInner()->GetEdgesWithNode(inner_node_);
    }

    EdgeType Get() override {
      assert(!IsAtEnd());
      if (!inner_node_edge_iterator_->IsAtEnd()) {
        auto inner_edge = inner_node_edge_iterator_->Get();
        return {parent_->Join(outer_node_, inner_edge.first),
                parent_->Join(outer_node_, inner_edge.second)};
      }

      assert(long_edge_.has_value());
      return *long_edge_;
    }

    void Next() override {
      assert(!IsAtEnd());
      if (long_edge_.has_value()) {
        is_at_end_ = true;
        return;
      }

      inner_node_edge_iterator_->Next();
      if (inner_node_edge_iterator_->IsAtEnd())
        long_edge_ = GetExtraEdge();
    }

    bool IsAtEnd() override { return is_at_end_; }

  private:
    EdgeType GetExtraEdge() {
      LOG_METHOD_ENTRY_AND_EXIT();

      long index = 0;
      EdgeType outer_edge;
      bool found_outer_edge = false;

      for (auto e :
           Iterate(parent_->GetOuter()->GetEdgesWithNode(outer_node_))) {
        if (index++ == inner_node_) {
          outer_edge = e;
          found_outer_edge = true;
          break;
        }
      }

      assert(found_outer_edge &&
             "Outer node must have degree equal to the number of nodes in the "
             "inner graph!");

      if (outer_edge.first != outer_node_)
        std::swap(outer_edge.first, outer_edge.second);

      assert(outer_edge.first == outer_node_);
      auto other_outer_node = outer_edge.second;

      auto inner_iterator = parent_->GetInner()->GetNodes();
      for (auto e :
           Iterate(parent_->GetOuter()->GetEdgesWithNode(other_outer_node))) {
        if (e.first == outer_node_ || e.second == outer_node_)
          break;
        inner_iterator->Next();
      }

      auto other_inner_node = inner_iterator->Get();
      return {parent_->Join(outer_node_, inner_node_),
              parent_->Join(other_outer_node, other_inner_node)};
    }

    ReplacementProduct *parent_;

    NodeType inner_node_;
    NodeType outer_node_;

    std::unique_ptr<EdgeIterator> inner_node_edge_iterator_;

    std::optional<EdgeType> long_edge_;
    bool is_at_end_ = false;
  };

  std::unique_ptr<EdgeIterator> GetEdgesWithNode(NodeType n) {
    return std::make_unique<ReplacementProductEdgeIterator>(this, /*node=*/n);
  }

  NodeCountType GetNodeCount() {
    return outer_->GetNodeCount() * inner_->GetNodeCount();
  }

  std::pair<NodeType, NodeType> Split(NodeType n) {
    NodeType outer = n / inner_->GetNodeCount();
    NodeType inner = n % inner_->GetNodeCount();

    return {outer, inner};
  }

  NodeType Join(NodeType outer, NodeType inner) {
    return outer * inner_->GetNodeCount() + inner;
  }

  ReplacementProduct(std::unique_ptr<Graph> outer,
                     std::unique_ptr<Graph> inner) {
    outer_ = std::move(outer);
    inner_ = std::move(inner);
    assert(IsRegular(outer_.get()).has_value());
    assert(IsRegular(inner_.get()).has_value());
  }

  Graph *GetOuter() { return outer_.get(); }
  Graph *GetInner() { return inner_.get(); }

private:
  std::unique_ptr<Graph> outer_;
  std::unique_ptr<Graph> inner_;
};
} // namespace

std::unique_ptr<Graph> CreateReplacementProduct(std::unique_ptr<Graph> outer,
                                                std::unique_ptr<Graph> inner) {
  return std::make_unique<ReplacementProduct>(std::move(outer),
                                              std::move(inner));
}

std::unique_ptr<Graph> CreateRingGraph(int n) {
  std::vector<Graph::EdgeType> edges;
  for (int i = 0; i < (n - 1); i++)
    edges.push_back({i, i + 1});
  edges.push_back({n - 1, 0});

  return CreateConcreteGraph(n, edges);
}
} // namespace graph
