#pragma once

#include <memory>

namespace graph {
class RandomBitGenerator {
public:
  virtual bool Generate() = 0;

  virtual ~RandomBitGenerator();
};

std::unique_ptr<RandomBitGenerator>
CreateDefaultRandomBitGenerator(unsigned seed);
} // namespace graph
