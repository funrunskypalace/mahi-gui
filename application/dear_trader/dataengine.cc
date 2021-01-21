#include "pch/compat.h"

#include "dataengine.h"

// convert

DataEngine::DataEngine() {}

float DataEngine::indexFromUpdateMillisec(int tradingDay, int64_t nanoTime) {
    auto mstime  = nanoTime / 1000000;
    auto msfirst = first_nano_ / 1000000;
    return (float)((mstime - msfirst) / 100) / (float)10000.0;
}

/// <summary>
/// 完全可以变成通用的通过JSON传递插件信息来实现插件化调用的接口。
/// 无需再使用成员变量current_plugin_。
/// </summary>
/// <param name="json"></param>
void DataEngine::processCommand(const std::string& json) {
    if (nullptr != current_plugin_) {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(mux_instruments_);
        instruments_.clear();
    }
    if (nullptr == current_plugin_) {
        std::string plugindir, pluginid, logdir;
        int log_level = imsimple::info, console_level = imsimple::err;
        try {
            nlohmann::json cfg = nlohmann::json::parse(json);
            if (!cfg.contains("plugin")) {
                auto log = fmt::format("Failed to find 'plugin' in JSON!");
                reportLog(warn, log);
                return;
            }
            cfg["id"].get_to(pluginid);
            cfg["console_level"].get_to(console_level);
            cfg["log_level"].get_to(log_level);
            cfg["logdir"].get_to(logdir);
            fix_path(logdir);
            cfg["hostdir"].get_to(plugindir);
            fix_path(plugindir);
        } catch (const std::exception&) {
            auto log = fmt::format("Failed to parse command son: {}", json);
            reportLog(warn, log);
            return;
        }

        current_plugin_ = plugin::load_plugin_dynamically(plugindir.c_str(), logdir.c_str(),
                                                          log_level, console_level, pluginid);
        if (nullptr == current_plugin_) {
            reportLog(warn, fmt::format("Failed to load plugin {}", pluginid));
            return;
        }
        current_plugin_->init(pluginid);
    }
    current_plugin_->doCommandAsync(
        json, [&](int32_t retval, const std::string& cmdName, const std::string& retmsg) -> int {
            if ("loadticks" == cmdName) {
                if (retmsg.size() != sizeof(ORDepthMarketDataField)) {
                    return -1;
                }
                auto depth = reinterpret_cast<const ORDepthMarketDataField*>(retmsg.data());
                if (0 == start_trading_day_) {
                    start_trading_day_ = depth->TradingDay;
                    first_nano_        = parse_milliseconds(depth->TradingDay, 90000000, false);
                }
                auto dataitm = std::make_shared<ORDepthMarketDataField>();
                memcpy(dataitm.get(), depth, sizeof(ORDepthMarketDataField));
                updateData(depth->ExchangeID, depth->InstrumentID,
                           [&](InstrumentPackPtr& pack) -> void {
                               pack->market_data.push_back(dataitm);
                               auto date = depth->TradingDay;
                               auto mstm = parse_milliseconds(date, depth->UpdateMillisec, false);
                               pack->buffer.AddPoint(indexFromUpdateMillisec(date, mstm),
                                                     depth->LastPrice);
                           });
            } else if ("exportticks" == cmdName) {
                reportLog(retval == 0 ? info : warn, cmdName + "," + retmsg);
            }
            return 0;
        });
}

void DataEngine::reportLog(int level, const std::string& msg) {
    PLOG(level, "{}", msg);
    std::lock_guard<std::mutex> lock(mux_logs_);
    auto retlog    = std::make_shared<LogInfoItem>();
    retlog->level  = level;
    retlog->logmsg = msg;
    logs_.push_back(retlog);
}

void DataEngine::iterate(SafeDataPackCb callback) {
    std::lock_guard<std::mutex> lock(mux_instruments_);
    for (auto pack : instruments_) {
        callback(pack.second);
    }
}

void DataEngine::updateData(const std::string& exid, const std::string& insid,
                            SafeDataPackCb callback) {
    InstrumentPackPtr pack = nullptr;
    {
        std::lock_guard<std::mutex> lock(mux_instruments_);

        auto it = instruments_.find(insid);
        if (it == instruments_.cend()) {
            auto pack   = std::make_shared<InstrumentPack>();
            pack->exid  = exid;
            pack->insid = insid;
            instruments_.insert(std::make_pair(insid, pack));
            it = instruments_.find(insid);
        }
        pack = it->second;
    }
    {
        std::lock_guard<std::mutex> lock(pack->mux_);
        callback(pack);
    }
}

int DataEngine::instrumentSize() const {
    std::lock_guard<std::mutex> lock(mux_instruments_);
    return instruments_.size();
}

int DataEngine::parseJson() { return 0; }

void DataEngine::fetchLogs(SafeFetchLogCb callback, bool clear) {
    std::lock_guard<std::mutex> lock(mux_logs_);
    for (auto log : logs_) {
        callback(log);
    }
    if (clear) {
        logs_.clear();
    }
}

void DataEngine::convertHdf5Eadb(const std::string& json) {}