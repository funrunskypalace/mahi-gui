#pragma once

#include "framework/market/filter_context.h"
#include "framework/market/mkt_plugin.h"
#include "framework/strategy/instance.h"
#include "framework/common/logger.h"

namespace framework
{

class GreatWallApp
{
public:
    GreatWallApp() { logger_ = createLogger("greatwall"); }

    virtual ~GreatWallApp() = default;

    template <typename... Args>
    void log(int level, const char* fmt, const Args&... args)
    {
        logger_->log(level, fmt, args...);
    }

    /// 如果配置文件中定义了多个过滤器，则会被多次调用。
    virtual market::MarketPluginPtr
    onInitializeMarketPluginInstance(const MarketPluginId& pluginId,
                                     const MarketPluginInstId& pluginInstId,
                                     market::FilterContextPtr& context) PURE;

    /// 如果配置文件中定义了多个策略实例，则会被多次调用。
    virtual strat::InstancePtr
    onInitializeStrategyInstance(const StrategyId& strategyId,
                                 const StrategyInstId& strategyInstId,
                                 strat::ContextPtr& context) PURE;

    /// 系统模块状态通知
    virtual void onSystemStatus(const std::string& moduleName,
                                const std::string& status,
                                const std::string& statusMsg){};

    /// 账号初始化状态变化之后推送
    virtual void onInvestorStatus(or_channelid_i channelId,
                                  or_investorid_i investorId, bool hasLogin){};

private:
    static GreatWallLoggerPtr createLogger(const std::string& loggerName);

private:
    GreatWallLoggerPtr logger_;
};
using GreatWallAppPtr = std::shared_ptr<GreatWallApp>;

} // namespace framework