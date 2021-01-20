#include "pch/compat.h"

#include "logger_wrapper.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

std::shared_ptr<spdlog::logger> LoggerWrapper::getLogger(const std::string& logger_name) {
    static std::map<std::string, std::shared_ptr<spdlog::logger> > map_;

    if (map_.find(logger_name) == map_.end()) {
        map_[logger_name] = createLogger(logger_name);
    }

    return map_[logger_name];
}

std::shared_ptr<spdlog::logger> LoggerWrapper::createLogger(const std::string& logger_name) {
    std::shared_ptr<spdlog::logger> logger;

    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink;
    std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> file_sink;

	std::vector<spdlog::sink_ptr> sinks;
	console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::info);
	sinks.push_back(console_sink);

	file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(fmt::format("./../logs/{}.log", logger_name), 200 * 1024 * 1024, 2);
	sinks.push_back(file_sink);

	logger = std::make_shared<spdlog::logger>(logger_name, std::begin(sinks), std::end(sinks));
	logger->set_level(spdlog::level::debug);
	logger->set_pattern("[%Y-%m-%d %H:%M:%S.%F][%t][%^%l%$] %v");
	logger->flush_on(spdlog::level::debug);

    return logger;
}