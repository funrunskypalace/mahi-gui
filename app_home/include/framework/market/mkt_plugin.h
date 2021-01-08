// @author Jiang Sanyi 2019-12-03
#pragma once

#include "framework/common/common.h"
#include "framework/market/filter_context.h"
#include "framework/market/market.h"
#include "framework/common/logger.h"

#include "framework/market/mkt_plugin-inl.h"

namespace framework
{
namespace market
{

/**
 * �û���չ���鴦�����û����жԽ��ⲿ���飬ִ��������չ�����
 * ����������������ܡ�
 *
 * �������������Ҫ��MarketPlugin�������ɿ��������
 */
class MarketPlugin
{
public:
    MarketPlugin(FilterContextPtr context) : context_(context){};

    virtual ~MarketPlugin(){};

    virtual MarketPluginId globalUniqueName() PURE;

    virtual int load(const std::string& jsonContent = "") PURE;

    virtual MarketPluginInstId instanceId() const
    {
        return context_ == nullptr ? "" : context_->uuid();
    };

    FilterContextPtr context() const { return context_; }

    template <typename... Args>
    void log(int level, const char* fmt, const Args&... args)
    {
        if (nullptr == context_)
        {
            return;
        }
        context_->logger()->log(level, fmt, args...);
    }

    virtual void subscribe(int dataType, const std::string& exId,
                           const std::string& insId) PURE;

    virtual void unsubscribe(int dataType, const std::string& exId,
                             const std::string& insId) PURE;

    virtual void onData(int32_t dataType, const char* data,
                        size_t datasiz) PURE;

    /// Ĭ�ϲ�ִ���κβ�������֧�����в��ԡ�
    virtual void onUnload(std::string& reason){};

    int unload(std::string& reason)
    {
        onUnload(reason);
        log(info, "market plugin {} unloaded", instanceId());
        if (nullptr != context_)
        {
            context_->stop();
            context_ = nullptr;
        }
        return F_NOERR;
    }

protected:
    /// ͳһ���ڷ���Context����
    FilterContextPtr context_;
};
using MarketPluginPtr = std::shared_ptr<MarketPlugin>;
} // namespace market
} // namespace framework
