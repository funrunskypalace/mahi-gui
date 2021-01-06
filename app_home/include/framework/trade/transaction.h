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

/// 交易执行事务。同一个事务内部包含多个交易指令，需要全部交易指令到达
/// 预定目标状态，当前事务才能进入completed状态。
class Transaction
{
public:
    virtual ~Transaction() = default;

    virtual std::string uuid() const PURE;

    virtual std::string description() const PURE;

    /// 按place的先后顺序定义每个委托的优先级。
    virtual Transaction& placeOrder(const OrderPackInfoPtr& order) PURE;

    virtual int32_t size() const PURE;

    /// 得到委托列表的当前镜像状态。线程安全。
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

    /// 更新事务交易属性。
    virtual void updateProp(const TransactionProp& newProp) PURE;

    /// 事务是否已经成交（已结束）
    virtual bool filled() const PURE;

    /// 当前处于废单状态（不一定结束）。
    virtual bool rejected() const PURE;

    /// 事务是否处于最终委托状态（不一定结束）
    virtual bool finalized() const PURE;

    /// 事务是否已经处于结束状态，且未成交。
    virtual bool terminated() const PURE;

    /// 整个事务是否结束（并处于成交或者中止）。
    virtual bool finished(bool includeTerminated) const
    {
        return includeTerminated ? (filled() || terminated()) : filled();
    }

    /// 事务是否需要等待当前委托成交。
    virtual bool pending() const PURE;

    virtual bool hasRiskControl() const PURE;

    /// 风控失败则返回false。
    virtual bool riskCheck() const PURE;
};
using TransactionPtr = std::shared_ptr<Transaction>;

} // namespace trade
} // namespace framework