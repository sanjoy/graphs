#include "counting.hpp"

#include "logging.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <set>
#include <vector>

namespace kb {
namespace {
class GraphCounter {
public:
  GraphCounter(unsigned order, unsigned degree)
      : order_(order), degree_(degree) {
    num_neighbors_.reset(new unsigned[order_]);
    assert((degree_ * order_) % 2 == 0);
    max_edges_ = (degree_ * order_) / 2;
    edges_.reset(new std::pair<unsigned, unsigned>[max_edges_]);
  }

  unsigned long Count() {
    RecursivelyAddEdge();
    return unique_graphs_.size();
  }

private:
  std::pair<unsigned, unsigned> Incr(std::pair<unsigned, unsigned> e) {
    e.second++;
    if (e.second == order_) {
      e.first++;
      e.second = e.first + 1;
    }
    return e;
  }

  bool IsAtEnd(std::pair<unsigned, unsigned> e) {
    return e.first == order_ - 1;
  }

  bool IsGraphRegular() {
    for (unsigned i = 0; i < order_; i++)
      num_neighbors_[i] = 0;
    for (int i = 0; i < max_edges_; i++) {
      num_neighbors_[edges_[i].first]++;
      num_neighbors_[edges_[i].second]++;
    }

    for (unsigned i = 0; i < order_; i++)
      if (num_neighbors_[i] != degree_)
        return false;
    return true;
  }

  void GetEdgesAsBitset(std::vector<bool> *result,
                        const std::vector<int> &permutation) {
    result->assign(order_ * order_, false);
    for (int i = 0; i < max_edges_; i++) {
      std::pair<unsigned, unsigned> e = {permutation[edges_[i].first],
                                         permutation[edges_[i].second]};
      (*result)[e.first * order_ + e.second] = true;
      (*result)[e.second * order_ + e.first] = true;
    }
  }

  void CountGraphIfRegularAndUnique() {
    if (!IsGraphRegular())
      return;
    std::vector<int> permutation(static_cast<int>(order_));
    std::iota(permutation.begin(), permutation.end(), 0);

    std::vector<bool> result(static_cast<int>(order_ * order_));
    bool unique = true;
    do {
      GetEdgesAsBitset(&result, permutation);
      if (unique_graphs_.count(result)) {
        unique = false;
        break;
      }
    } while (std::next_permutation(permutation.begin(), permutation.end()));

    if (unique) {
      auto inserted = unique_graphs_.insert(result).second;
      assert(inserted);
      (void)inserted;
      if (IsLoggingEnabled()) {
        std::cerr << "graph G {\n";
        for (int i = 0; i < max_edges_; i++)
          std::cerr << "  " << edges_[i].first << " -- " << edges_[i].second
                    << "\n";
        std::cerr << "}\n";
      }
    }
  }

  void RecursivelyAddEdge(int depth = 0,
                          std::pair<unsigned, unsigned> start = {0, 1}) {
    if (depth == max_edges_) {
      CountGraphIfRegularAndUnique();
      return;
    }

    for (std::pair<unsigned, unsigned> i = start; !IsAtEnd(i); i = Incr(i)) {
      edges_[depth] = i;
      RecursivelyAddEdge(depth + 1, Incr(i));
    }
  }

  std::unique_ptr<unsigned[]> num_neighbors_;
  std::unique_ptr<std::pair<unsigned, unsigned>[]> edges_;
  std::set<std::vector<bool>> unique_graphs_;

  unsigned long order_;
  unsigned long degree_;
  unsigned long max_edges_;
};
} // namespace

unsigned long CountRegularGraphsWithDegree(unsigned order, unsigned degree) {
  assert(order > degree);
  GraphCounter gc(order, degree);
  return gc.Count();
}
} // namespace kb
