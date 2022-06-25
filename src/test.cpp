#include "test.hpp"

#include <iostream>

namespace graph {
static bool AnyAssertFailedFlag = false;

void AssertFailed(const char *msg, const char *file, int line) {
  std::cerr << msg << " failed @ " << file << ":" << line << std::endl;
  AnyAssertFailedFlag = true;
}

bool AnyAssertFailed() { return AnyAssertFailedFlag; }
void ClearAssertFailed() { AnyAssertFailedFlag = false; }
} // namespace graph
