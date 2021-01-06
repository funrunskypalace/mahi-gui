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
    /// ��������ʱʱ�䣨΢�룩��
    std::atomic<int32_t> TimeoutMicroseconds;
    /// ��ʱ���Ƿ�ִ�г���ָ�������������Ϊcompleted״̬��
    std::atomic<bool> CancelOnTimeout;
    /// ��ʱ���Ƿ���ѳɽ�ί��ִ��ƽ��ָ�������������Ϊcompleted״̬��
    std::atomic<bool> CloseOnTimeout;
    /// ���ճ��������ʱ��
    std::atomic<int32_t> ExposureDeadlineMicroseconds;
    std::atomic<int32_t> MaxCancelTimes;
    std::atomic<int32_t> CancelIntervalMicro;
    std::atomic<int32_t> MaxSendTimes;
    std::atomic<bool> TerminateOnRejected;
    std::atomic<bool> KeepWaitingForLastResponse;
    /// �������
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