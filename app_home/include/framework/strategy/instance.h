#pragma once

#include "framework/strategy/context.h"
#include "framework/trade/transaction.h"

#include "framework/strategy/instance-inl.h"

namespace framework
{
namespace strat
{

/// ˵����ʵ��ͬһ���߳�ʵ��onData��onExecStarted�Ȼص���
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

    /// ��ʼִ������
    /// @note: �벻Ҫ����д������ִ�д�����ʱ����ί���ڻص���ɺ�Żᷢ�͡�
    virtual void onExecStarted(const trade::TransactionPtr& transaction){};

    /// ����ִ�й����е�״̬���ա�
    virtual void onExecSnapshot(const trade::TransactionPtr& transaction,
                                OROrderFieldPtr order){};

    /// ����ִ�й����еĳɽ����ա�
    virtual void onExecSnapshot(const trade::TransactionPtr& transaction,
                                ORTradeFieldPtr trade){};

    /// ִ��������ɡ�ͨ�������terminated()��filled()������ȡ����ִ�н����״̬��
    /// @note: �벻Ҫ����д������ִ�д�����ʱ����
    virtual void onExecFinished(const trade::TransactionPtr& transaction){};

    /// ����timerId������ע���Timer��������
    /// return false to kill timer.
    virtual bool onTimer(int32_t timerId)
    {
        (void)timerId;
        return true;
    }

    /// @param: clearCounter - for test
    /// ÿ�η�����ʱ�򶼻���øú�����
    /// NOTE:
    /// �ú����ɽ���ִ�����ڲ��̵߳��ã������̺߳�onData/onExecFinished�Ȼص�
    /// �̶߳���һ����
    virtual or_price_f
    updateOrderPrice(const trade::OrderPackInfoPtr& orderPack,
                     const trade::TransactionPtr& transaction,
                     bool clearCounter)
    {
        return orderPack->limitPrice();
    }

    /// Ĭ�ϲ�ִ���κβ�������֧�����в��ԡ�
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
    /// Ŀǰͳһʹ�øö���ֱ�ӷ���Context��
    ContextPtr context_;
};
using InstancePtr = std::shared_ptr<Instance>;
} // namespace strat
} // namespace framework
