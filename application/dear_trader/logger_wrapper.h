#pragma once

#include "spdlog/spdlog.h"
#include "fmt/format.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include <map>
#include <memory>
#include <string>

class LoggerWrapper {
public:
    static std::shared_ptr<spdlog::logger> createLogger(const std::string& logger_name);
    static std::shared_ptr<spdlog::logger> getLogger(const std::string& logger_name);
};