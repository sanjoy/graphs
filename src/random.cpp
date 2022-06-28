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

namespace {
unsigned long FastDiceRoller(RandomBitGenerator *gen,
                             unsigned long upper_bound) {
  // Lumbroso, Jérémie. "Optimal discrete uniform generation from coin
  // flips, and applications." arXiv preprint arXiv:1304.1916 (2013).
  unsigned long v = 1, c = 0;
  while (true) {
    v *= 2;
    c = 2 * c + (gen->Generate() ? 1 : 0);
    if (v >= upper_bound) {
      if (c < upper_bound) {
        return c;
      } else {
        v = v - upper_bound;
        c = c - upper_bound;
      }
    }
  }
}
} // namespace

unsigned long GenerateRandomInteger(RandomBitGenerator *gen,
                                    unsigned long upper_bound) {
  return FastDiceRoller(gen, upper_bound);
}
} // namespace graph
