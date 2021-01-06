#pragma once

#include <list>

#include "framework/common/data_type.h"
#include "oos/ORDataType.h"

using namespace cycnoches::ort;

namespace framework
{

struct StrategySdk
{
    std::string server_addr;
};

struct TradingCode
{
    or_investorid_i investor_id;
    std::string exchange_id;
    std::string trading_code;
};

struct MarketFacilityConfig
{
    int32_t is_active;
    int32_t type;
    std::string type_name;
    std::string unique_name;
    std::string config_path;
    std::string custom_config;
};

struct StrategyInstanceConfig
{
    int32_t is_active;
    std::string strategy_id;
    std::string strategy_inst_id;
    std::string operation_user_info;
    std::string config_file;
    std::string config_json;
    OR_RESUME_TYPE resume_type;
};

struct MarketPluginInstanceConfig
{
    int32_t is_active;
    std::string plugin_id;
    std::string plugin_inst_id;
    std::string operation_user_info;
    std::string config_file;
    std::string config_json;
    OR_RESUME_TYPE resume_type;
};

struct StatsConfig
{
    int32_t order_new_delay_ms;
};

#ifdef USE_OPENORDERAPI

struct OperatorAccount
{
    int32_t is_active;
    int64_t operator_id;
    std::string password;
    std::string subscribe_tids;
};

struct OosFront
{
    int32_t is_active;
    std::string unique_name;
    std::string address;
    OperatorAccount operator_account;
};

struct GreatWallConfig
{
    std::string stop_time;
    /// logger path
    std::string log_path;
    /// flow data path
    std::string data_path;
    std::string resource_path;
    std::list<MarketFacilityConfig> market_handlers;
    std::list<MarketPluginInstanceConfig> market_plugin_instances;
    std::list<StrategyInstanceConfig> strategy_instances;
    OosFront front;
    StrategySdk strategy_sdk;
    StatsConfig stats;
};

#else

struct Investor
{
    or_investorid_i investor_id;
    std::string user_id;
    std::string account_code;
    std::string password;
    std::string broker_id;
    std::string client_id;
    std::string investor_type;
    std::list<TradingCode> trading_codes;
};

struct TradeInterface
{
    int32_t is_active;
    or_channelid_i channel_id;
    or_channeltype_e channel_type;
    std::string channel_loadid;
    or_channelloadtype_e channel_loadtype;
    std::string type_info;
    std::string type_name;
    std::string unique_name;
    std::string ipv4_address;
    int32_t ipv4_port;
    int resume_type; // 0 - restart, 1 - resume, 2 - quick. 2 not supported yet.
    bool enable_trade;
    bool enable_market;
    std::string custom_config;
    std::list<Investor> investors;
};

struct GreatWallConfig
{
    std::string stop_time;
    /// logger path
    std::string log_path;
    /// flow data path
    std::string data_path;
    std::string resource_path;
    std::string auth_code;
    std::string license_path;
    std::string instance_config_path;
    std::list<TradeInterface> trade_interfaces;
    StrategySdk strategy_sdk;
    std::list<MarketFacilityConfig> market_handlers;
    std::list<MarketPluginInstanceConfig> market_plugin_instances;
    std::list<StrategyInstanceConfig> strategy_instances;
    std::list<std::string> startup_commands;
    StatsConfig stats;
};
#endif

using GreatWallConfigPtr = std::shared_ptr<GreatWallConfig>;

} // namespace framework