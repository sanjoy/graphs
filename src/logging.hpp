#pragma once

#include <iostream>

namespace graph {
void EnableLogging();
void DisableLogging();
bool IsLoggingEnabled();

#define LOG                                                                    \
  if (IsLoggingEnabled())                                                      \
  std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "

#define LOG_VAR(var)                                                           \
  do {                                                                         \
    LOG << #var << " = " << var << std::endl;                                  \
  } while (0)

} // namespace graph
