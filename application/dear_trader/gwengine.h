#pragma once

#include <string>
#include <map>
#include <future>
#include <atomic>

#include "framework/router/trade_engine_base.h"

using namespace cycnoches::ort;
using namespace framework;
using namespace framework::router;

struct JrSubscribeItem {
    std::string       exId;
    std::string       insId;
    or_productclass_e productClass;
};

using or_md_data_list_ptr = std::shared_ptr<std::list<ORDepthMarketDataFieldPtr>>;

class GwTraderEngine : public TradeEngineBase {
    OrderRouterApi*     or_api_;
    std::atomic<bool>   inited_      = {false};
    std::atomic<bool>   exiting_     = {false};
    or_md_data_list_ptr new_md_data_ = {nullptr};
    std::mutex          mux_md_data_;

public:
    GwTraderEngine(const GreatWallConfigPtr& configs) : TradeEngineBase(configs) {}

    void init();
    void join();
    void stop();
    void subscribe(const std::string& exId, const std::string& insId,
                   or_productclass_e product_class);
    void unsubscribe(const std::string& exId, const std::string& insId,
                     or_productclass_e product_class);

    or_md_data_list_ptr fetch();

protected:
    virtual void notifyTradeEngineStatus(const std::string& statusCode,
                                         const std::string& statusMsg) override;
    virtual void onLogin(or_channelid_i channelId, or_investorid_i investorId,
                         bool hasLogin) override;
    virtual void onInitialized(or_channelid_i channelId, or_investorid_i investorId,
                               bool isSucceed) override;
    virtual void onDepthMarketData(or_channelid_i          channelId,
                                   ORDepthMarketDataField* pDepthMarketData) override;

private:
};
using GwTraderEnginePtr = std::shared_ptr<GwTraderEngine>;
extern GwTraderEnginePtr gwengine_main(int argc, char** argv);
