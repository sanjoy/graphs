#pragma once

#include <iostream>

namespace graph {
void AssertFailed(const char *msg, const char *file, int line);
bool AnyAssertFailed();
void ClearAssertFailed();
} // namespace graph

#define CHECK(cond)                                                            \
  if (!(cond)) {                                                               \
    AssertFailed(#cond, __FILE__, __LINE__);                                   \
    return;                                                                    \
  }

#define __IMPL__RUN_TEST(fn)                                                   \
  do {                                                                         \
    ClearAssertFailed();                                                       \
    std::cout << "Running " << #fn << std::endl;                               \
    fn();                                                                      \
    if (AnyAssertFailed()) {                                                   \
      std::cout << "Failed " << #fn << std::endl;                              \
      failed_tests.push_back(#fn);                                             \
    } else                                                                     \
      std::cout << "Passed " << #fn << std::endl;                              \
  } while (0);

#define RUN_MAIN(TEST_LIST)                                                    \
  do {                                                                         \
    std::vector<std::string> failed_tests;                                     \
    TEST_LIST(__IMPL__RUN_TEST);                                               \
    return failed_tests.empty() ? 0 : 1;                                       \
  } while (0)
