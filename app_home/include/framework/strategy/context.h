#pragma once

#include "framework/server/strategy_manager.h"
#include "framework/trade/transaction.h"
#include "framework/common/logger.h"

using namespace cycnoches::ort;

namespace framework
{
namespace strat
{

/// ÿ�����Ի���һ��ר����Context��
/// ÿ��Context����ͬʱ���������񡣲���Ӧ������������
class Context
{
public:
    /// ͨ��Context������������
    virtual trade::TransactionPtr
    createTransaction(const trade::TransactionProp& prop) PURE;

    virtual std::string strategy_id() const PURE;

    virtual std::string uuid() const PURE;

    template <typename... Args>
    void log(int level, const char* fmt, const Args&... args)
    {
        if (nullptr == logger())
            return;
        logger()->log(level, fmt, args...);
    }

    virtual GreatWallLoggerPtr logger() const PURE;

    virtual int32_t setTimer(int32_t intervalMillisecs) PURE;

    virtual void killTimer(int32_t timerId) PURE;

    virtual int terminate(const trade::TransactionPtr& transaction) PURE;

    /// numTransactions����Ϊ0��ʾ��������
    virtual void setCapacity(int numTransactions) PURE;

    virtual int32_t capacity() const PURE;

    virtual int32_t size() const PURE;

    /// ��ȡ��������صĽ���ͨ�����˺��Ƿ��Ѿ����Խ��׵�״̬��
    /// Ŀǰ������ΪContext��ָ��ͨ����Ϣ��������Ҫ���ݸ���Transaction���ж�״̬��
    virtual bool isTradeReady(trade::TransactionPtr& transaction,
                              std::string& statusMsg) PURE;

    virtual bool isTradeReady(or_channelid_i channelId,
                              or_investorid_i investorId,
                              std::string& statusMsg) PURE;

    virtual int orderInsert(trade::TransactionPtr& transaction) PURE;
    virtual int orderInsert(trade::TransactionPtr& transaction,
                            std::string& errMsg) PURE;

    virtual int orderCancel(trade::TransactionPtr& transaction, bool force,
                            std::string& errMsg) PURE;

    // virtual ORAccountPositionFieldPtr getPosition(const std::string& exId,
    // 	const std::string& insId) const PURE;
    // virtual trade::OrderPackInfoPtr getOrderInsert(const std::string& exId,
    // 	const std::string& insId, or_posidirection_e posiDirection) const PURE;
    // virtual trade::OrderPackInfoPtr getOrderAction(const std::string& exId,
    // 	const std::string& insId, or_posidirection_e posiDirection) const PURE;

    virtual void subscribe(int dataType, const std::string& exId,
                           const std::string& insId) PURE;

    virtual void unsubscribe(int dataType, const std::string& exId,
                             const std::string& insId) PURE;

    virtual ORAccountPositionFieldPtr
    getPosition(or_investorid_i investorId, const std::string& exId,
                const std::string& insId, or_positiondate_e posDate) const PURE;

    virtual std::shared_ptr<std::list<ORAccountPositionFieldPtr>>
    getAllPositions() const PURE;

    virtual ORAccountAssetFieldPtr
    getAccountAsset(or_investorid_i investorId) const PURE;

#ifdef ENABLE_MANAGE_SDK
    virtual server::StrategyManagerPtr strategyManager() const PURE;
#endif
};
using ContextPtr = std::shared_ptr<Context>;

} // namespace strat
} // namespace framework
