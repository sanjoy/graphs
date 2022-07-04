#include "logging.hpp"

namespace kb {
static thread_local bool LoggingFlag = false;

void EnableLogging() { LoggingFlag = true; }

void DisableLogging() { LoggingFlag = false; }

bool IsLoggingEnabled() { return LoggingFlag; }

EnableLoggingInScope::EnableLoggingInScope() {
  if (!IsLoggingEnabled()) {
    active_ = true;
    EnableLogging();
  }
}

EnableLoggingInScope::~EnableLoggingInScope() {
  if (active_)
    DisableLogging();
}

namespace detail {
static thread_local int MethodNesting;

LogMethodEntryAndExit::LogMethodEntryAndExit(bool enabled, const char *method)
    : enabled_(enabled), method_(method) {
  if (enabled_)
    LOG << "Entering " << method_ << "\n";
  MethodNesting++;
}

/*static*/ std::string LogMethodEntryAndExit::GetNestingIndentation() {
  std::string nesting;
  nesting.append(MethodNesting, ' ');
  return nesting;
}

LogMethodEntryAndExit::~LogMethodEntryAndExit() {
  MethodNesting--;
  if (enabled_)
    LOG << "Exiting " << method_ << "\n";
}
} // namespace detail
} // namespace kb
