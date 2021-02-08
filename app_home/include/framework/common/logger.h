#pragma once

#include "fmt/format.h"
#include "framework/common/common.h"

namespace framework {

enum level_enum {
    trace    = 0,
    debug    = 1,
    info     = 2,
    warn     = 3,
    err      = 4,
    critical = 5,
    off      = 6,
};

/**
 * Automatically created logger for each strategy instance.
 */
class GreatWallLogger {
public:
    GreatWallLogger() = default;
    template <typename... Args>
    void log(int level, const char* fmt, const Args&... args) {
        log_(level, fmt::format(fmt, args...));
    }

    virtual void log_(int level, const std::string& log) PURE;
};

using GreatWallLoggerPtr = std::shared_ptr<GreatWallLogger>;

// has module name: "[%Y-%m-%d %T.%e][%t][%l][%n] %v"
extern void greatwall_init_global_log(const char* logPathName, const char* loggerName, int logLevel,
                                      int consoleLevel, const std::string& logFormat);

}  // namespace framework