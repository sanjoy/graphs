#pragma once

#include <memory>
#include <optional>
#include <span>
#include <utility>

namespace kb {
class Graph {
public:
  using VertexTy = unsigned long;
  using OrderTy = unsigned long;
  using EdgeTy = std::pair<VertexTy, VertexTy>;

  virtual ~Graph();

  class VertexIterator {
  public:
    virtual VertexTy Get() = 0;
    virtual void Next() = 0;
    virtual bool IsAtEnd() = 0;

    virtual ~VertexIterator();
  };

  class EdgeIterator {
  public:
    virtual EdgeTy Get() = 0;
    virtual void Next() = 0;
    virtual bool IsAtEnd() = 0;

    virtual ~EdgeIterator();
  };

  virtual std::unique_ptr<EdgeIterator>
  GetEdgesContainingVertex(VertexTy v) = 0;

  virtual OrderTy GetOrder() = 0;

  virtual std::unique_ptr<VertexIterator> GetVertices();
  virtual std::unique_ptr<EdgeIterator> GetEdges();

  virtual std::unique_ptr<Graph> Clone() = 0;
};

namespace detail {

class AdaptedIteratorSentinel {};

template <typename T, typename It> class AdaptedIterator {
public:
  AdaptedIterator(It *it) : iterator_(it) {}

  T operator*() { return iterator_->Get(); }

  void operator++() { iterator_->Next(); }

  bool operator==(AdaptedIteratorSentinel) { return iterator_->IsAtEnd(); }

private:
  It *iterator_;
};

template <typename T, typename It> class IteratorAdapter {
public:
  IteratorAdapter(std::unique_ptr<It> it) : iterator_(std::move(it)) {}

  AdaptedIterator<T, It> begin() {
    return AdaptedIterator<T, It>{iterator_.get()};
  }

  AdaptedIteratorSentinel end() { return AdaptedIteratorSentinel{}; }

private:
  std::unique_ptr<It> iterator_;
};
} // namespace detail

inline detail::IteratorAdapter<Graph::EdgeTy, Graph::EdgeIterator>
Iterate(std::unique_ptr<Graph::EdgeIterator> it) {
  return detail::IteratorAdapter<Graph::EdgeTy, Graph::EdgeIterator>(
      std::move(it));
}

inline detail::IteratorAdapter<Graph::VertexTy, Graph::VertexIterator>
Iterate(std::unique_ptr<Graph::VertexIterator> it) {
  return detail::IteratorAdapter<Graph::VertexTy, Graph::VertexIterator>(
      std::move(it));
}

std::unique_ptr<Graph> CreateConcreteGraph(Graph::OrderTy num_vertices,
                                           std::span<Graph::EdgeTy> edges);

std::optional<std::string> CheckConsistency(Graph *g);

std::ostream &operator<<(std::ostream &, const Graph::EdgeTy &);
} // namespace kb
