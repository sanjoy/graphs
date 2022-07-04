#include "graph_zoo.hpp"

#include "graph_analysis.hpp"
#include "logging.hpp"

#include <vector>

namespace kb {
std::unique_ptr<Graph> CreateCompleteGraph(int k, bool self_loops) {
  std::vector<Graph::EdgeTy> edges;
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
  std::vector<Graph::EdgeTy> edges;
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
    ReplacementProductEdgeIterator(ReplacementProduct *parent, VertexTy vertex)
        : parent_(parent) {
      std::tie(outer_vertex_, inner_vertex_) = parent_->Split(vertex);
      inner_vertex_edge_iterator_ =
          parent_->GetInner()->GetEdgesContainingVertex(inner_vertex_);
    }

    EdgeTy Get() override {
      assert(!IsAtEnd());
      if (!inner_vertex_edge_iterator_->IsAtEnd()) {
        auto inner_edge = inner_vertex_edge_iterator_->Get();
        return {parent_->Join(outer_vertex_, inner_edge.first),
                parent_->Join(outer_vertex_, inner_edge.second)};
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

      inner_vertex_edge_iterator_->Next();
      if (inner_vertex_edge_iterator_->IsAtEnd())
        long_edge_ = GetExtraEdge();
    }

    bool IsAtEnd() override { return is_at_end_; }

  private:
    EdgeTy GetExtraEdge() {
      LOG_METHOD_ENTRY_AND_EXIT();

      long index = 0;
      EdgeTy outer_edge;
      bool found_outer_edge = false;

      for (auto e : Iterate(
               parent_->GetOuter()->GetEdgesContainingVertex(outer_vertex_))) {
        if (index++ == inner_vertex_) {
          outer_edge = e;
          found_outer_edge = true;
          break;
        }
      }

      assert(found_outer_edge && "Outer vertex must have degree equal to the "
                                 "number of vertices in the "
                                 "inner graph!");

      if (outer_edge.first != outer_vertex_)
        std::swap(outer_edge.first, outer_edge.second);

      assert(outer_edge.first == outer_vertex_);
      auto other_outer_vertex = outer_edge.second;

      auto inner_iterator = parent_->GetInner()->GetVertices();
      for (auto e : Iterate(parent_->GetOuter()->GetEdgesContainingVertex(
               other_outer_vertex))) {
        if (e.first == outer_vertex_ || e.second == outer_vertex_)
          break;
        inner_iterator->Next();
      }

      auto other_inner_vertex = inner_iterator->Get();
      return {parent_->Join(outer_vertex_, inner_vertex_),
              parent_->Join(other_outer_vertex, other_inner_vertex)};
    }

    ReplacementProduct *parent_;

    VertexTy inner_vertex_;
    VertexTy outer_vertex_;

    std::unique_ptr<EdgeIterator> inner_vertex_edge_iterator_;

    std::optional<EdgeTy> long_edge_;
    bool is_at_end_ = false;
  };

  std::unique_ptr<EdgeIterator> GetEdgesContainingVertex(VertexTy v) override {
    return std::make_unique<ReplacementProductEdgeIterator>(this, /*vertex=*/v);
  }

  OrderTy GetOrder() override {
    return outer_->GetOrder() * inner_->GetOrder();
  }

  std::pair<VertexTy, VertexTy> Split(VertexTy v) {
    VertexTy outer = v / inner_->GetOrder();
    VertexTy inner = v % inner_->GetOrder();

    return {outer, inner};
  }

  VertexTy Join(VertexTy outer, VertexTy inner) {
    return outer * inner_->GetOrder() + inner;
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

  std::unique_ptr<Graph> Clone() override {
    return std::make_unique<ReplacementProduct>(outer_->Clone(),
                                                inner_->Clone());
  }

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

std::unique_ptr<Graph> CreateRingGraph(int v) {
  std::vector<Graph::EdgeTy> edges;
  for (int i = 0; i < (v - 1); i++)
    edges.push_back({i, i + 1});
  edges.push_back({v - 1, 0});

  return CreateConcreteGraph(v, edges);
}
} // namespace kb
