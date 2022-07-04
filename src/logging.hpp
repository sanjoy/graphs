#pragma once

#include <iostream>

namespace kb {
void EnableLogging();
void DisableLogging();
bool IsLoggingEnabled();

class EnableLoggingInScope {
public:
  EnableLoggingInScope();
  ~EnableLoggingInScope();

private:
  bool active_ = false;
};

namespace detail {
class LogMethodEntryAndExit {
public:
  LogMethodEntryAndExit(bool enabled, const char *method);

  ~LogMethodEntryAndExit();

  static std::string GetNestingIndentation();

private:
  bool enabled_;
  const char *method_;
};
} // namespace detail

#define __LOG_PREFIX                                                           \
  "[" << __FILE__ << ":" << __LINE__ << "] "                                   \
      << ::kb::detail::LogMethodEntryAndExit::GetNestingIndentation()

#define LOG                                                                    \
  if (IsLoggingEnabled())                                                      \
  std::cerr << __LOG_PREFIX

#define LOG_IF(cond)                                                           \
  if (IsLoggingEnabled() || (cond))                                            \
  std::cerr << __LOG_PREFIX

#define LOG_VAR(var)                                                           \
  do {                                                                         \
    LOG << #var << " = " << var << std::endl;                                  \
  } while (0)

#define LOG_VAR_IF(cond, var)                                                  \
  do {                                                                         \
    LOG_IF(cond) << #var << " = " << var << std::endl;                         \
  } while (0)

#define LOG_METHOD_ENTRY_AND_EXIT()                                            \
  ::kb::detail::LogMethodEntryAndExit __log_entry_and_exit {                   \
    IsLoggingEnabled(), __PRETTY_FUNCTION__                                    \
  }

} // namespace kb
