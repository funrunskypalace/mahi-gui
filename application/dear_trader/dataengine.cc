#include "pch/compat.h"

#include "dataengine.h"

// convert

DataEngine::DataEngine() {}

float DataEngine::indexFromUpdateMillisec(int tradingDay, int64_t nanoTime) {
    auto mstime  = nanoTime / 1000000;
    auto msfirst = first_nano_ / 1000000;
    return (float)((mstime - msfirst) / 100) / (float)10000.0;
}

void DataEngine::loadMarketData(const std::string& json) {
    if (hdf5_plugin_) {
        hdf5_plugin_->close();
        hdf5_plugin_ = nullptr;
        instruments_.clear();
    }
    if (nullptr == hdf5_plugin_) {
        hdf5_plugin_ = plugin::load_plugin_dynamically("D:/prod/win64/implugin/app_home/plugins",
            "./../logs/", imsimple::info, imsimple::err, "hdf5reader");
        if (nullptr == hdf5_plugin_)
            return;
        hdf5_plugin_->init("hdf5reader");
    }
    hdf5_plugin_->doCommandAsync(json, [&](int32_t retval, const std::string& cmdName, const std::string& retmsg) -> int {

        if ("loadticks" == cmdName) {
            if (retmsg.size() != sizeof(ORDepthMarketDataField)) {
                return -1;
            }
            auto depth = reinterpret_cast<const ORDepthMarketDataField*>(retmsg.data());
            if (0 == start_trading_day_) {
                start_trading_day_ = depth->TradingDay;
                first_nano_ = parse_milliseconds(depth->TradingDay, 90000000, false);
            }
            auto dataitm = std::make_shared<ORDepthMarketDataField>();
            memcpy(dataitm.get(), depth, sizeof(ORDepthMarketDataField));
            updateData(depth->ExchangeID, depth->InstrumentID, [&](InstrumentPackPtr& pack) -> void {
                pack->market_data.push_back(dataitm);
                auto date = depth->TradingDay;
                auto mstm = parse_milliseconds(date, depth->UpdateMillisec, false);
                pack->buffer.AddPoint(indexFromUpdateMillisec(date, mstm), depth->LastPrice);
                });
        }
        else if ("exportticks" == cmdName) {
            std::lock_guard<std::mutex> lock(mux_logs_);
            auto retlog = std::make_shared<LogInfoItem>();
            retlog->level = retval == 0 ? info : warn;
            retlog->logmsg = retmsg;
            logs_.push_back(retlog);
        }
        return 0;
    });
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