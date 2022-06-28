#include "random.hpp"

#include <random>

namespace graph {
namespace {
class DefaultRandomBitGenerator final : public RandomBitGenerator {
public:
  DefaultRandomBitGenerator(unsigned seed)
      : generator_(seed), distribution_(0.5) {}

  bool Generate() override { return distribution_(generator_); }

private:
  std::default_random_engine generator_;
  std::bernoulli_distribution distribution_;
};
} // namespace

RandomBitGenerator::~RandomBitGenerator() {}

std::unique_ptr<RandomBitGenerator>
CreateDefaultRandomBitGenerator(unsigned seed) {
  return std::make_unique<DefaultRandomBitGenerator>(seed);
}
} // namespace graph
