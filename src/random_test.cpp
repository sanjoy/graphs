#include "random.hpp"
#include "test.hpp"

#include <vector>

using namespace graph;

static std::vector<unsigned long>
GenerateRandomIntHistogram(unsigned long range, unsigned long count) {
  auto gen = CreateDefaultRandomBitGenerator();

  std::vector<unsigned long> histogram(range);
  for (unsigned long i = 0; i < count; i++)
    histogram[GenerateRandomInteger(gen.get(), range)]++;

  return histogram;
}

static void TestGenerateRandomInteger_Range10_Count50() {
  auto histogram = GenerateRandomIntHistogram(/*range=*/10, /*count=*/50);
  CHECK_EQ(histogram[0], 5);
  CHECK_EQ(histogram[1], 3);
  CHECK_EQ(histogram[2], 5);
  CHECK_EQ(histogram[3], 9);
  CHECK_EQ(histogram[4], 5);
  CHECK_EQ(histogram[5], 4);
  CHECK_EQ(histogram[6], 5);
  CHECK_EQ(histogram[7], 6);
  CHECK_EQ(histogram[8], 5);
  CHECK_EQ(histogram[9], 3);
}

static void TestGenerateRandomInteger_Range100_Count5() {
  auto histogram = GenerateRandomIntHistogram(/*range=*/100, /*count=*/5);
  for (int i = 0; i < 100; i++) {
    bool should_be_one = i == 8 || i == 21 || i == 88 || i == 95 || i == 99;
    if (should_be_one)
      CHECK_EQ(histogram[i], 1);
    else
      CHECK_EQ(histogram[i], 0);
  }
}

#define TEST_LIST(F)                                                           \
  F(TestGenerateRandomInteger_Range10_Count50)                                 \
  F(TestGenerateRandomInteger_Range100_Count5)                                 \
  (void)0;

DEFINE_MAIN(TEST_LIST)
