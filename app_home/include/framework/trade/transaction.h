#pragma once

#include "framework/common/common.h"
#include "oos/OpenOrderApi.h"
//#include "pch/compat.h"

#include <memory>

#include "framework/trade/order_exec.h"
#include "framework/trade/transaction_prop.h"

using namespace cycnoches::ort;

namespace framework
{
namespace trade
{

using OROrderList = std::list<OROrderFieldPtr>;

/// ����ִ������ͬһ�������ڲ������������ָ���Ҫȫ������ָ���
/// Ԥ��Ŀ��״̬����ǰ������ܽ���completed״̬��
class Transaction
{
public:
    virtual ~Transaction() = default;

    virtual std::string uuid() const PURE;

    virtual std::string description() const PURE;

    /// ��place���Ⱥ�˳����ÿ��ί�е����ȼ���
    virtual Transaction& placeOrder(const OrderPackInfoPtr& order) PURE;

    virtual int32_t size() const PURE;

    /// �õ�ί���б�ĵ�ǰ����״̬���̰߳�ȫ��
    virtual OROrderList
    getOrdersSnapshot(or_execstatus_i statusMixed) const PURE;

    virtual int32_t getMaxCancelTimes() const PURE;

    virtual int32_t getCancelIntervalMicro() const PURE;

    virtual int32_t getTimeoutMicroseconds() const PURE;

    virtual int32_t getMaxSendTimes() const PURE;

    virtual int32_t getCancelTimes() const PURE;

    virtual int32_t getReSendTimes() const PURE;

    virtual bool getTerminateOnRejected() const PURE;

    virtual bool keepWaitingForLastResponse() const PURE;

    /// �������������ԡ�
    virtual void updateProp(const TransactionProp& newProp) PURE;

    /// �����Ƿ��Ѿ��ɽ����ѽ�����
    virtual bool filled() const PURE;

    /// ��ǰ���ڷϵ�״̬����һ����������
    virtual bool rejected() const PURE;

    /// �����Ƿ�������ί��״̬����һ��������
    virtual bool finalized() const PURE;

    /// �����Ƿ��Ѿ����ڽ���״̬����δ�ɽ���
    virtual bool terminated() const PURE;

    /// ���������Ƿ�����������ڳɽ�������ֹ����
    virtual bool finished(bool includeTerminated) const
    {
        return includeTerminated ? (filled() || terminated()) : filled();
    }

    /// �����Ƿ���Ҫ�ȴ���ǰί�гɽ���
    virtual bool pending() const PURE;

    virtual bool hasRiskControl() const PURE;

    /// ���ʧ���򷵻�false��
    virtual bool riskCheck() const PURE;
};
using TransactionPtr = std::shared_ptr<Transaction>;

} // namespace trade
} // namespace framework