#include "gwengine.h"
#include "framework/router/parser.h"
#include "framework/common/logger.h"
#include "ort/libor.h"

#include <thread>
#include <filesystem>

std::shared_ptr<spdlog::logger> LOGGER = nullptr;

const char* DEFAULT_LOG_FORMAT = "[%Y-%m-%d %T.%e][%t][%l][%n] %v";

// =====================================================
//
// =====================================================

void GwTraderEngine::notifyTradeEngineStatus(const std::string& statusCode,
                                             const std::string& statusMsg) {
    LOGGER->info(">>> Got trading engine notify >>>  {} {}", statusCode, statusMsg);
}

void GwTraderEngine::onLogin(or_channelid_i channelId, or_investorid_i investorId, bool hasLogin) {
    LOGGER->info("onLogin {}, {}, {}", channelId, investorId, hasLogin);
}

void GwTraderEngine::onInitialized(or_channelid_i channelId, or_investorid_i investorId,
                                   bool isSucceed) {
    LOGGER->info("(InitControl) channel {} investor {} initialized {}.", channelId, investorId,
                 isSucceed ? "success" : "failed");

    init();
}

void GwTraderEngine::onDepthMarketData(or_channelid_i          channelId,
                                       ORDepthMarketDataField* pDepthMarketData) {
    LOGGER->info("onDepthMarketData channelId={}, insId={}", channelId,
                 pDepthMarketData->InstrumentID);
    auto depth = std::make_shared<ORDepthMarketDataField>();
    memcpy(depth.get(), pDepthMarketData, sizeof(ORDepthMarketDataField));
    std::lock_guard<std::mutex> lock(mux_md_data_);
    new_md_data_->push_back(depth);
}

int product_class_to_market_data(or_productclass_e product_class) {
    switch (product_class) {
        case '3': return 100;
        case '1': return 500;
        case '2': return 300;
        default: return 100;
    }
}

or_productclass_e market_data_to_product_class(int market_data) {
    switch (market_data) {
        case 100: return '3';
        case 500: return '1';
        case 300: return '2';
        default: return '3';
    }
}

// =====================================================
// GwTraderEngine
// =====================================================
void GwTraderEngine::init() {
    int curdate = get_current_date(false);
    make_directory_tree(appConfigs()->data_path.c_str());
    new_md_data_ = std::make_shared<std::list<ORDepthMarketDataFieldPtr>>();
}

void GwTraderEngine::join() {
    while (!exiting_) {
        SleepMs(1000);
    }
    LOGGER->info("GwTraderEngine joined.");
}

void GwTraderEngine::stop() { exiting_ = true; }

void GwTraderEngine::subscribe(const std::string& exId, const std::string& insId,
                               or_productclass_e product_class) {
    auto channel = getChannel(11001);
    if (nullptr == channel) {
        LOGGER->warn("Channel not ready!");
        return;
    }

    // ORDepthMarketDataField field;
    // strcpy(field.ExchangeID, "SH");
    // assign(field.InstrumentID, "600000", OR_INSTRUMENTID_LEN);
    // callback_(&field);
    // return;

    auto field = std::make_shared<ORReqSubscribeMarketDataField>();
    assign(field->ExchangeID, exId.c_str(), OR_EXCHANGEID_LEN);
    assign(field->InstrumentID, insId.c_str(), OR_INSTRUMENTID_LEN);
    field->ChannelID       = 11001;
    field->ProductClass    = product_class;
    field->MarketDataType  = product_class_to_market_data(product_class);
    field->SubscribeSwitch = OR_RS_ON;
    channel->subscribeMarket(field.get());
}

void GwTraderEngine::unsubscribe(const std::string& exId, const std::string& insId,
                                 or_productclass_e product_class) {
    auto channel = getChannel(11001);
    if (nullptr == channel) {
        LOGGER->warn("Channel not ready!");
        return;
    }

    auto field = std::make_shared<ORReqSubscribeMarketDataField>();
    assign(field->ExchangeID, exId.c_str(), OR_EXCHANGEID_LEN);
    assign(field->InstrumentID, insId.c_str(), OR_INSTRUMENTID_LEN);
    field->ChannelID       = 11001;
    field->ProductClass    = product_class;
    field->MarketDataType  = product_class_to_market_data(product_class);
    field->SubscribeSwitch = OR_RS_OFF;
    channel->subscribeMarket(field.get());
}

or_md_data_list_ptr GwTraderEngine::fetch() {
    std::lock_guard<std::mutex> lock(mux_md_data_);

    auto trans   = new_md_data_;
    new_md_data_ = std::make_shared<std::list<ORDepthMarketDataFieldPtr>>();
    return trans;
}
// =====================================================
// GwTraderEngine
// =====================================================

GwTraderEnginePtr gwengine_main(int argc, char** argv) {
    char* pfn = "../config/GreatWall.json";
    if (argc >= 2) {
        if (!is_file_exists(argv[1])) {
            std::cerr << "Usage: <JrDataDump> [<GreatWall.json path>, default "
                         "`../config/GreatWall-jr_datadump.json`]"
                      << std::endl;
            return 0;
        } else {
            pfn = argv[1];
        }
    }
    if (!is_file_exists(pfn)) {
        std::cerr << fmt::format("GreatWall config file {} not exists!", pfn) << std::endl;
        return nullptr;
    }
    auto confcontent = read_whole_file(pfn);

    std::string errmsgs;

    auto appconfigs = parse_config_json_text(confcontent, errmsgs);
    if (!errmsgs.empty()) {
        std::cerr << fmt::format("parse config file warn: {}", errmsgs) << std::endl;
        return nullptr;
    }

    auto orfn = fmt::format("{}teor.GwTraderEngineer.log", appconfigs->log_path);
    g_initLibOrderRouter(orfn.c_str(), spdlog::level::debug, false, DEFAULT_LOG_FORMAT);
    auto gwfn = fmt::format("{}greatwall.GwTraderEngineer.log", appconfigs->log_path);
    greatwall_init_global_log(gwfn.c_str(), "greatwall", debug, true, DEFAULT_LOG_FORMAT);

    LOGGER = LoggerWrapper::getLogger("GwTraderEngine");
    LOGGER->info("Using GreatWall config file: {}", pfn);
    auto dumper = std::make_shared<GwTraderEngine>(appconfigs);
    dumper->init();

    LOGGER->info("Starting trader...");
    dumper->start(RunMode::Execute, "gwengine");
    LOGGER->info("trader started.");
    return dumper;
}
