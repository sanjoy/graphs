#include "logging.hpp"

namespace graph {
bool LoggingFlag = false;

void EnableLogging() { LoggingFlag = true; }

void DisableLogging() { LoggingFlag = false; }

bool IsLoggingEnabled() { return LoggingFlag; }
} // namespace graph
