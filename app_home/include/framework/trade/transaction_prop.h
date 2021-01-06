#pragma once

#include "framework/common/common.h"

namespace framework
{
namespace strat
{ // avoid recursive including
class RcBase;
using RcBasePtr = std::shared_ptr<RcBase>;
} // namespace strat
namespace trade
{

struct TransactionProp
{
    /// 交易事务超时时间（微秒）。
    std::atomic<int32_t> TimeoutMicroseconds;
    /// 超时后是否执行撤单指令。若否则立即置为completed状态。
    std::atomic<bool> CancelOnTimeout;
    /// 超时后是否对已成交委托执行平仓指令。若否则立即置为completed状态。
    std::atomic<bool> CloseOnTimeout;
    /// 风险敞口最后交易时间
    std::atomic<int32_t> ExposureDeadlineMicroseconds;
    std::atomic<int32_t> MaxCancelTimes;
    std::atomic<int32_t> CancelIntervalMicro;
    std::atomic<int32_t> MaxSendTimes;
    std::atomic<bool> TerminateOnRejected;
    std::atomic<bool> KeepWaitingForLastResponse;
    /// 风控设置
    strat::RcBasePtr RiskControl;

    TransactionProp()
    {
        TimeoutMicroseconds = 0;
        CancelOnTimeout = false;
        CloseOnTimeout = false;
        ExposureDeadlineMicroseconds = 0;
        MaxCancelTimes = 1;
        CancelIntervalMicro = 100 * 1000;
        MaxSendTimes = 1;
        RiskControl = nullptr;
        TerminateOnRejected = false;
        KeepWaitingForLastResponse = true;
    }
    TransactionProp(const TransactionProp& other) { update(other); }
    void update(const TransactionProp& other)
    {
        TimeoutMicroseconds.store(other.TimeoutMicroseconds);
        CancelOnTimeout.store(other.CancelOnTimeout);
        CloseOnTimeout.store(other.CloseOnTimeout);
        ExposureDeadlineMicroseconds.store(other.ExposureDeadlineMicroseconds);
        MaxCancelTimes.store(other.MaxCancelTimes);
        CancelIntervalMicro.store(other.CancelIntervalMicro);
        MaxSendTimes.store(other.MaxSendTimes);
        RiskControl = other.RiskControl;
        TerminateOnRejected.store(other.TerminateOnRejected);
        KeepWaitingForLastResponse.store(other.KeepWaitingForLastResponse);
    }
    std::string description() const
    {
        return fmt::format(
            "timeout(us):{}, sendtimes:{}, canceltimes:{}, cancelinterval:{}, exposure(us):{}, docancel:{}, doclose:{}, TerminateOnRejected:{}, KeepWaitingForLastResponse:{}, {} riskcontrol.",
            TimeoutMicroseconds, MaxSendTimes, MaxCancelTimes,
            CancelIntervalMicro, ExposureDeadlineMicroseconds, CancelOnTimeout,
            CloseOnTimeout, TerminateOnRejected, KeepWaitingForLastResponse,
            nullptr == RiskControl ? "has" : "no");
    }
}; // namespace trade

} // namespace trade
} // namespace framework