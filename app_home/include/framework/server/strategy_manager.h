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
    /// @note: 请不要在重写函数中执行大量耗时处理。委托在回调完成后才会发送。
    virtual void
    notifyExecStarted(const std::string strategyId,
                      const std::string stratInstId,
                      const trade::TransactionPtr& transaction) PURE;

    /// 事务执行过程中的状态快照。
    virtual void notifyExecSnapshot(const std::string strategyId,
                                    const std::string stratInstId,
                                    const trade::TransactionPtr& transaction,
                                    OROrderFieldPtr order) PURE;

    /// 事务执行过程中的成交快照。
    virtual void notifyExecSnapshot(const std::string strategyId,
                                    const std::string stratInstId,
                                    const trade::TransactionPtr& transaction,
                                    ORTradeFieldPtr trade) PURE;

    /// 执行事务完成。通过事务的terminated()或filled()方法获取事务执行结果的状态。
    /// @note: 请不要在重写函数中执行大量耗时处理。
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
