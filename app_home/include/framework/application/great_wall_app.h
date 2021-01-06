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

    /// ��������ļ��ж����˶������������ᱻ��ε��á�
    virtual market::MarketPluginPtr
    onInitializeMarketPluginInstance(const MarketPluginId& pluginId,
                                     const MarketPluginInstId& pluginInstId,
                                     market::FilterContextPtr& context) PURE;

    /// ��������ļ��ж����˶������ʵ������ᱻ��ε��á�
    virtual strat::InstancePtr
    onInitializeStrategyInstance(const StrategyId& strategyId,
                                 const StrategyInstId& strategyInstId,
                                 strat::ContextPtr& context) PURE;

    /// ϵͳģ��״̬֪ͨ
    virtual void onSystemStatus(const std::string& moduleName,
                                const std::string& status,
                                const std::string& statusMsg){};

    /// �˺ų�ʼ��״̬�仯֮������
    virtual void onInvestorStatus(or_channelid_i channelId,
                                  or_investorid_i investorId, bool hasLogin){};

private:
    static GreatWallLoggerPtr createLogger(const std::string& loggerName);

private:
    GreatWallLoggerPtr logger_;
};
using GreatWallAppPtr = std::shared_ptr<GreatWallApp>;

} // namespace framework