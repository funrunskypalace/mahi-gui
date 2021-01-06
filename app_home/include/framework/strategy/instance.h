#pragma once

#include "framework/strategy/context.h"
#include "framework/trade/transaction.h"

#include "framework/strategy/instance-inl.h"

namespace framework
{
namespace strat
{

/// 说明：实现同一个线程实现onData、onExecStarted等回调。
class Instance
{
public:
    Instance(ContextPtr context) : context_(context){};

    virtual ~Instance() = default;

    virtual StrategyId globalUniqueName() PURE;

    template <typename... Args>
    void log(int level, const char* fmt, const Args&... args)
    {
        if (nullptr == context_)
        {
            return;
        }
        context_->logger()->log(level, fmt, args...);
    }

    virtual int load(const std::string& jsonContent = "") PURE;

    virtual StrategyInstId instanceId() const
    {
        return context_ == nullptr ? "" : context_->uuid();
    };

    ContextPtr context() const { return context_; }

    virtual bool updateParameters(const std::string& jsonContent)
    {
        return true;
    }

    /// make a public data feed method.
    virtual void onData(int32_t dataType, const char* data,
                        size_t datasiz) PURE;

    /// 开始执行事务。
    /// @note: 请不要在重写函数中执行大量耗时处理。委托在回调完成后才会发送。
    virtual void onExecStarted(const trade::TransactionPtr& transaction){};

    /// 事务执行过程中的状态快照。
    virtual void onExecSnapshot(const trade::TransactionPtr& transaction,
                                OROrderFieldPtr order){};

    /// 事务执行过程中的成交快照。
    virtual void onExecSnapshot(const trade::TransactionPtr& transaction,
                                ORTradeFieldPtr trade){};

    /// 执行事务完成。通过事务的terminated()或filled()方法获取事务执行结果的状态。
    /// @note: 请不要在重写函数中执行大量耗时处理。
    virtual void onExecFinished(const trade::TransactionPtr& transaction){};

    /// 根据timerId来区分注册的Timer触发器。
    /// return false to kill timer.
    virtual bool onTimer(int32_t timerId)
    {
        (void)timerId;
        return true;
    }

    /// @param: clearCounter - for test
    /// 每次发单的时候都会调用该函数。
    /// NOTE:
    /// 该函数由交易执行器内部线程调用，与主线程和onData/onExecFinished等回调
    /// 线程都不一样。
    virtual or_price_f
    updateOrderPrice(const trade::OrderPackInfoPtr& orderPack,
                     const trade::TransactionPtr& transaction,
                     bool clearCounter)
    {
        return orderPack->limitPrice();
    }

    /// 默认不执行任何操作，以支持已有策略。
    virtual void onUnload(std::string& reason){};

    int unload(std::string& reason)
    {
        log(info, "Start unload strategy {}", instanceId());
        onUnload(reason);
        log(info, "Strategy instance {} unloaded", instanceId());
        context_ = nullptr;
        return F_NOERR;
    }

protected:
    /// 目前统一使用该对象直接访问Context。
    ContextPtr context_;
};
using InstancePtr = std::shared_ptr<Instance>;
} // namespace strat
} // namespace framework
