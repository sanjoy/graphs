#pragma once

#include <memory>

namespace kb {
class RandomBitGenerator {
public:
  virtual bool Generate() = 0;

  virtual ~RandomBitGenerator();
};

std::unique_ptr<RandomBitGenerator>
CreateDefaultRandomBitGenerator(unsigned seed = 1);

unsigned long GenerateRandomInteger(RandomBitGenerator *gen,
                                    unsigned long upper_bound);
} // namespace kb
