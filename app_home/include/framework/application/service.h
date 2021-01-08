#pragma once

#include "orchid/common/callback.h"
#include "framework/application/great_wall_app.h"
#include "framework/common/common.h"
#include "framework/strategy/instance.h"
#include "framework/market/mkt_plugin.h"

using namespace framework::market;
using namespace orchid::Common;

namespace framework
{

enum class RunMode : int
{
    Execute,
    NoExecute,
    Test
};

struct StrategyRegistryInfo
{
    GreatWallAppPtr application;
    StrategyInstanceConfig strategy_instance_config;
};
using StrategyRegistryInfoPtr = std::shared_ptr<StrategyRegistryInfo>;
using StrategyRegistry = std::unordered_map<StrategyInstId /*strategy_inst_id*/,
                                            StrategyRegistryInfoPtr>;
using StrategyRegistryPtr = std::shared_ptr<StrategyRegistry>;

struct MarketPluginRegistryInfo
{
    GreatWallAppPtr application;
    MarketPluginInstanceConfig market_plugin_instance_config;
};
using MarketPluginRegistryInfoPtr = std::shared_ptr<MarketPluginRegistryInfo>;
using MarketPluginRegistry =
    std::unordered_map<MarketPluginInstId, MarketPluginRegistryInfoPtr>;
using MarketPluginRegistryPtr = std::shared_ptr<MarketPluginRegistry>;

using ServiceRunCb = std::function<void()>;
/**
 * Service �����ڽ��̼����ṩ���ԵĲ�����һ��Service����ά���������ʵ����
 * ����ͨ������ID��globalUniqueName,��strategyId�����࣬ͨ������ʵ��ID(strategyInstId)
 * ����ȫ��(��Խ���Service)Ψһ��ʶ��
 */
class Service
{
public:
    static std::shared_ptr<Service> createService(GreatWallAppPtr appInst,
                                                  int argc, char** argv);

    virtual ~Service() = default;

    virtual const GreatWallConfigPtr appConfigs() const PURE;

    virtual int run(RunMode runMode, ServiceRunCb preRunCB = nullptr) PURE;

    virtual int stop() PURE;

    virtual bool needQuit() const PURE;

    virtual void join() PURE;

    virtual bool ready() const PURE;

    virtual int registerMarketPluginInstance(
        std::string& errMsg, const MarketPluginId& pluginId,
        MarketPluginInstId& pluginInstId, const std::string& configFile,
        const std::string& configJson, const std::string& operationUserInfo,
        OR_RESUME_TYPE resumeType) PURE;

    /// ֧��Lazy load:
    /// ����ڲ�����������ǰ��ע�ᣬ������������ͬʱ�Զ�����һ�����ԡ�
    /// ����ڲ�������������ע�ᣬ����Զ�̬�������ԡ�
    virtual int
    registerStrategyInstance(std::string& errMsg, const StrategyId& strategyId,
                             StrategyInstId& strategyInstId,
                             const std::string& configFile = "",
                             const std::string& configJson = "",
                             const std::string& operationUserInfo = "",
                             OR_RESUME_TYPE resumeType = OR_TERT_QUICK) PURE;

    virtual MarketPluginRegistryInfoPtr
    getMarketPluginRegistry(const MarketPluginInstId& pluginInstId) const PURE;

    virtual StrategyRegistryInfoPtr
    getStrategyInstanceRegistry(const StrategyInstId& strategyId) const PURE;

    virtual MarketPluginRegistryPtr getRegisteredMarketPlugins() const PURE;

    virtual StrategyRegistryPtr getRegisteredStrategyInstances() const PURE;

    virtual int startMarketPlugin(const MarketPluginId& pluginId,
                                  const MarketPluginInstId& pluginInstId) PURE;

    virtual int stopMarketPlugin(const MarketPluginId& pluginId,
                                 const MarketPluginInstId& pluginInstId,
                                 std::string& reason) PURE;

    virtual int startStrategy(const StrategyId& strategyId,
                              const StrategyInstId& strategyInstId) PURE;

    virtual int stopStrategy(const StrategyId& strategyId,
                             const StrategyInstId& strategyInstId,
                             std::string& reason) PURE;

    virtual strat::InstancePtr
    getStrategyInstance(const StrategyInstId& strategyInstId) PURE;

    /// globalUniqueName: same as instance_id in ManageSDK.
    virtual std::list<strat::InstancePtr>
    getStrategyInstances(const StrategyId& strategyId) PURE;
};
using ServicePtr = std::shared_ptr<Service>;

} // namespace framework