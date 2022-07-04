#include "test.hpp"

#include <iostream>

namespace kb {
namespace detail {
static bool AnyAssertFailedFlag = false;

void AssertFailed(const char *msg, const char *file, int line) {
  std::cerr << msg << " failed @ " << file << ":" << line << std::endl;
  AnyAssertFailedFlag = true;
}

bool AnyAssertFailed() { return AnyAssertFailedFlag; }
void ClearAssertFailed() { AnyAssertFailedFlag = false; }

void ParseCmdOptions(int argc, char **argv, TestOptions *out) {
#define CMD_ARG_STR_KEY_VALUE(sym)                                             \
  do {                                                                         \
    std::string sym_str = #sym;                                                \
    for (char &c : sym_str)                                                    \
      if (c == '_')                                                            \
        c = '-';                                                               \
    sym_str = "--" + sym_str + "=";                                            \
    if (arg.find(sym_str) == 0) {                                              \
      if (!out->sym.empty()) {                                                 \
        std::cerr << "Only one " << sym_str << " allowed!" << std::endl;       \
        abort();                                                               \
      }                                                                        \
      out->sym = arg.substr(sym_str.size());                                   \
    }                                                                          \
  } while (0)

#define CMD_ARG_BOOL(sym)                                                      \
  do {                                                                         \
    std::string sym_str = #sym;                                                \
    for (char &c : sym_str)                                                    \
      if (c == '_')                                                            \
        c = '-';                                                               \
    sym_str = "--" + sym_str;                                                  \
    if (arg == sym_str)                                                        \
      out->sym = true;                                                         \
  } while (0)

  for (int i = 0; i < argc; i++) {
    std::string arg = argv[i];

    CMD_ARG_BOOL(quiet_mode);
    CMD_ARG_BOOL(enable_logging);

    CMD_ARG_STR_KEY_VALUE(enable_logging_for_test);
    CMD_ARG_STR_KEY_VALUE(test_filter);
  }
}

void CanonicalizeEdgeList(std::vector<Graph::EdgeTy> *edges) {
  for (Graph::EdgeTy &e : *edges) {
    if (e.first > e.second)
      std::swap(e.first, e.second);
  }

  std::sort(edges->begin(), edges->end(),
            [](const Graph::EdgeTy &e1, const Graph::EdgeTy &e2) {
              if (e1.first == e2.first)
                return e1.second < e2.second;
              return e1.first < e2.first;
            });
}
} // namespace detail
} // namespace kb
