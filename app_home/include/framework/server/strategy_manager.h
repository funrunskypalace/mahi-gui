// @author Jiang Sanyi 2020-03-27
#pragma once

#include "framework/common/common.h"
#include "framework/trade/transaction.h"

namespace framework
{
namespace server
{

class StrategyManager
{
public:
    /// @note: �벻Ҫ����д������ִ�д�����ʱ����ί���ڻص���ɺ�Żᷢ�͡�
    virtual void
    notifyExecStarted(const std::string strategyId,
                      const std::string stratInstId,
                      const trade::TransactionPtr& transaction) PURE;

    /// ����ִ�й����е�״̬���ա�
    virtual void notifyExecSnapshot(const std::string strategyId,
                                    const std::string stratInstId,
                                    const trade::TransactionPtr& transaction,
                                    OROrderFieldPtr order) PURE;

    /// ����ִ�й����еĳɽ����ա�
    virtual void notifyExecSnapshot(const std::string strategyId,
                                    const std::string stratInstId,
                                    const trade::TransactionPtr& transaction,
                                    ORTradeFieldPtr trade) PURE;

    /// ִ��������ɡ�ͨ�������terminated()��filled()������ȡ����ִ�н����״̬��
    /// @note: �벻Ҫ����д������ִ�д�����ʱ����
    virtual void
    notifyExecFinished(const std::string strategyId,
                       const std::string stratInstId,
                       const trade::TransactionPtr& transaction) PURE;

    virtual void notifyUnload(const std::string strategyId,
                              const std::string stratInstId,
                              std::string& reason) PURE;

    virtual void notifyRealtimePriceDiff(const std::string& exId1,
                                         const std::string& insId1,
                                         const std::string& exId2,
                                         const std::string& insId2,
                                         double value_difference,
                                         double price_difference) PURE;

    virtual void notifyLogging(const std::string& strategy_id,
                               const std::string& strategy_ins_id,
                               int logging_level,
                               const std::string& logging_msg,
                               uint64_t logging_timestamp) PURE;

    virtual void notifyTransactionStatus(const std::string& strategy_id,
                                         const std::string& strategy_ins_id,
                                         const std::string& transaction_uuid,
                                         uint64_t logging_timestamp,
                                         char transaction_status,
                                         int execute_result,
                                         const std::string& exec_stats) PURE;
};
using StrategyManagerPtr = StrategyManager*;

} // namespace server
} // namespace framework
