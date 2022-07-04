#pragma once

#include <memory>
#include <optional>
#include <span>
#include <utility>

namespace graph {
class Graph {
public:
  using NodeType = unsigned long;
  using NodeCountType = unsigned long;
  using EdgeType = std::pair<NodeType, NodeType>;

  virtual ~Graph();

  class NodeIterator {
  public:
    virtual NodeType Get() = 0;
    virtual void Next() = 0;
    virtual bool IsAtEnd() = 0;

    virtual ~NodeIterator();
  };

  class EdgeIterator {
  public:
    virtual EdgeType Get() = 0;
    virtual void Next() = 0;
    virtual bool IsAtEnd() = 0;

    virtual ~EdgeIterator();
  };

  virtual std::unique_ptr<NodeIterator> GetNodes();
  virtual std::unique_ptr<EdgeIterator> GetEdges();
  virtual std::unique_ptr<EdgeIterator> GetEdgesWithNode(NodeType n) = 0;

  virtual NodeCountType GetNodeCount() = 0;

  virtual std::optional<std::string> CheckConsistency();
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

inline detail::IteratorAdapter<Graph::EdgeType, Graph::EdgeIterator>
Iterate(std::unique_ptr<Graph::EdgeIterator> it) {
  return detail::IteratorAdapter<Graph::EdgeType, Graph::EdgeIterator>(
      std::move(it));
}

inline detail::IteratorAdapter<Graph::NodeType, Graph::NodeIterator>
Iterate(std::unique_ptr<Graph::NodeIterator> it) {
  return detail::IteratorAdapter<Graph::NodeType, Graph::NodeIterator>(
      std::move(it));
}

std::unique_ptr<Graph> CreateConcreteGraph(Graph::NodeCountType num_nodes,
                                           std::span<Graph::EdgeType> edges);

std::optional<std::string> CheckConsistency(Graph *g);

std::ostream &operator<<(std::ostream &, const Graph::EdgeType &);
} // namespace graph
