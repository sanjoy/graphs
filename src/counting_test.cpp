#include "counting.hpp"

#include "logging.hpp"
#include "test.hpp"

using namespace kb;

static void TestCountRegularGraphsWithDegree_4_2() {
  CHECK_EQ(CountRegularGraphsWithDegree(4, 2), 1);
}

static void TestCountRegularGraphsWithDegree_6_3() {
  CHECK_EQ(CountRegularGraphsWithDegree(6, 3), 2);
}

static void TestCountRegularGraphsWithDegree_6_2() {
  CHECK_EQ(CountRegularGraphsWithDegree(6, 2), 2);
}

#define TEST_LIST(F)                                                           \
  F(TestCountRegularGraphsWithDegree_4_2)                                      \
  F(TestCountRegularGraphsWithDegree_6_3)                                      \
  F(TestCountRegularGraphsWithDegree_6_2)                                      \
  (void)0;

DEFINE_MAIN(TEST_LIST)
