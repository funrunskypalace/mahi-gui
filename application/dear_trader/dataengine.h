#pragma once

#include "ort/ORDataStruct.h"
#include "implugin/plugin.h"
#include "instrumentpack.h"
#include <functional>

using namespace imsimple;
using namespace cycnoches::ort;

struct LogInfoItem {
    int level;
    std::string logmsg;
};
using LogInfoItemPtr = std::shared_ptr<LogInfoItem>;

using SafeDataPackCb = std::function<void(InstrumentPackPtr&)>;
using SafeFetchLogCb = std::function<void(const LogInfoItemPtr&)>;

class DataEngine {
public:
    DataEngine();
    void processCommand(const std::string& json);
    void iterate(SafeDataPackCb callback);
    void updateData(const std::string& exid, const std::string& insid, SafeDataPackCb callback);
    int instrumentSize() const;
    void fetchLogs(SafeFetchLogCb callback, bool clear = true);
    void convertHdf5Eadb(const std::string& json);

protected:
    int parseJson();
    void reportLog(int level, const std::string& msg);
    float indexFromUpdateMillisec(int tradingDay, int64_t nanoTime);

private:
    mutable std::mutex mux_instruments_;
    std::unordered_map<std::string, InstrumentPackPtr> instruments_;
    plugin::AnyOtherPtr current_plugin_ = {nullptr};
    int start_trading_day_              = {0};
    int64_t first_nano_                 = {0};
    mutable std::mutex mux_logs_;
    std::list<LogInfoItemPtr> logs_;
};
using DataEnginePtr = std::shared_ptr<DataEngine>;
