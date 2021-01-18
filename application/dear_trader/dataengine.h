#pragma once

#include "ort/ORDataStruct.h"
#include "implugin/plugin.h"
#include "instrumentpack.h"
#include <functional>

using namespace imsimple;
using namespace cycnoches::ort;

using SafeDataPackCb = std::function<void(InstrumentPackPtr&)>;
using SafeFetchLogCb = std::function<void(const std::string&)>;

struct LogInfoItem {
    int level;
    std::string logmsg;
};
using LogInfoItemPtr=std::shared<LogInfoItem>;

class DataEngine {
public:
    DataEngine();
    void loadMarketData(const std::string& json);
    void iterate(SafeDataPackCb callback);
    void updateData(const std::string& exid, const std::string& insid, SafeDataPackCb callback);
    int instrumentSize() const;
    void fetchLogs(SafeFetchLogCb callback, bool clear = true);
    void convertHdf5Eadb(const std::string& json);

protected:
    int parseJson();
    float indexFromUpdateMillisec(int tradingDay, int64_t nanoTime);

private:
    mutable std::mutex mux_instruments_;
    std::unordered_map<std::string, InstrumentPackPtr> instruments_;
    plugin::AnyOtherPtr hdf5_plugin_ = {nullptr};
    int start_trading_day_           = {0};
    int64_t first_nano_              = {0};
    mutable std::mutex mux_logs_;
    std::list<LogInfoItemPtr> logs_;
};
using DataEnginePtr = std::shared_ptr<DataEngine>;
