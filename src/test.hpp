#pragma once

#include "logging.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace graph {
namespace detail {
void AssertFailed(const char *msg, const char *file, int line);
bool AnyAssertFailed();
void ClearAssertFailed();

struct TestOptions {
  bool quiet_mode = false;
  bool enable_logging = false;
  std::string enable_logging_for_test;
  std::string test_filter;
};

void ParseCmdOptions(int argc, char **argv, TestOptions *out);
} // namespace detail
} // namespace graph

#define CHECK(cond)                                                            \
  if (!(cond)) {                                                               \
    ::graph::detail::AssertFailed(#cond, __FILE__, __LINE__);                  \
    return;                                                                    \
  }

#define CHECK_HAS_VALUE_AND_ASSIGN(var, expr)                                  \
  auto __expr = expr;                                                          \
  CHECK(__expr.has_value());                                                   \
  var = *__expr;

#define PRINT_VAR(var)                                                         \
  do {                                                                         \
    std::cerr << #var " = " << var << std::endl;                               \
  } while (0)

#define PRINT_VECTOR(vect)                                                     \
  do {                                                                         \
    for (int i = 0, e = vect.size(); i != e; i++) {                            \
      std::cerr << #vect << "[" << i << "] = " << vect[i] << std::endl;        \
    }                                                                          \
  } while (0)

#define CHECK_EQ(a, b)                                                         \
  do {                                                                         \
    auto __a = (a);                                                            \
    auto __b = (b);                                                            \
    if (__a != __b) {                                                          \
      ::graph::detail::AssertFailed(#a " != " #b, __FILE__, __LINE__);         \
      std::cerr << #a << " = " << __a << std::endl;                            \
      std::cerr << #b << " = " << __b << std::endl;                            \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define __IMPL__RUN_TEST(fn)                                                   \
  do {                                                                         \
    if (opts.test_filter.empty() || #fn == opts.test_filter) {                 \
      if (#fn == opts.enable_logging_for_test || opts.enable_logging)          \
        ::graph::EnableLogging();                                              \
      ::graph::detail::ClearAssertFailed();                                    \
      if (!opts.quiet_mode)                                                    \
        std::cout << "Running " << #fn << std::endl;                           \
      fn();                                                                    \
      if (::graph::detail::AnyAssertFailed()) {                                \
        std::cout << "Failed " << #fn << std::endl;                            \
        failed_tests.push_back(#fn);                                           \
      } else if (!opts.quiet_mode)                                             \
        std::cout << "Passed " << #fn << std::endl;                            \
      ::graph::DisableLogging();                                               \
    }                                                                          \
  } while (0);

#define RUN_MAIN(TEST_LIST)                                                    \
  do {                                                                         \
    ::graph::detail::TestOptions opts;                                         \
    ParseCmdOptions(argc, argv, &opts);                                        \
    std::cerr.precision(std::numeric_limits<double>::max_digits10);            \
    std::vector<std::string> failed_tests;                                     \
    TEST_LIST(__IMPL__RUN_TEST);                                               \
    return failed_tests.empty() ? 0 : 1;                                       \
  } while (0)

#define DEFINE_MAIN(TEST_LIST)                                                 \
  int main(int argc, char **argv) { RUN_MAIN(TEST_LIST); }
