#pragma once

#include "framework/application/service.h"
#include "framework/common/common.h"

#include "oos/OpenOrderApi.h"

using namespace cycnoches::ort;

namespace framework
{
namespace router
{

class ChannelHooked
{
public:
    virtual ~ChannelHooked() = default;
    virtual void onConnected(){};
    virtual void onRtnOrder(or_channelid_i channelId,
                            or_orrequestid_i orRequestId,
                            OROrderField* pOrder){};
    virtual void onRtnTrade(or_channelid_i channelId,
                            or_orrequestid_i orRequestId,
                            ORTradeField* pTrade){};
    virtual void onErrRtnOrderSend(or_channelid_i channelId,
                                   OROrderReqField* pInputOrder,
                                   ORRspInfoField* pRspInfo){};
    virtual void onErrRtnOrderCancel(or_channelid_i channelId,
                                     OROrderCancelReqField* pOrderAction,
                                     ORRspInfoField* pRspInfo){};
    virtual void onLogin(or_channelid_i channelId, or_investorid_i investorId,
                         bool hasLogin){};
    /// 在登录之后，完成初始化查询和重演之后，回调。
    virtual void onInitialized(or_channelid_i channelId,
                               or_investorid_i investorId, bool isSucceed){};
    virtual void onDepthMarketData(or_channelid_i channelId,
                                   ORDepthMarketDataField* pDepthMarketData){};
    virtual void onRspQryAccountAsset(or_channelid_i channelId,
                                      ORAccountAssetField* pRspAccountAsset,
                                      ORRspInfoField* pRspInfo){};
    virtual void onRspQryPosition(or_channelid_i channelId,
                                  ORAccountPositionField* pRspPosition,
                                  ORRspInfoField* pRspInfo){};
    virtual void onRspQryPositionDtl(or_channelid_i channelId,
                                     ORAccountPositionDtlField* pRspPositionDtl,
                                     ORRspInfoField* pRspInfo){};

    virtual void onPositionChanged(or_channelid_i channelId,
                                   ORAccountPositionField* pRspPosition,
                                   ORRspInfoField* pRspInfo){};
    virtual void onAccountAssetChanged(or_channelid_i channelId,
                                       ORAccountAssetField* pRspPosition,
                                       ORRspInfoField* pRspInfo){};
};

class ChannelSpi :
#ifdef USE_OPENORDERAPI
    public OpenOrderSpi
#else
    public OrderRouterSpi
#endif
{
public:
    ChannelSpi(
#ifdef USE_OPENORDERAPI
#else
        or_channelid_i channelId,
#endif
        ChannelHooked* pHook);

protected:
    virtual void onConnected() override;
    virtual void onDisconnected(int nReason) override{};
    virtual void
    onCounterConnected(ORCounterConfigField* counterConfig) override;
    virtual void onCounterDisconnected(ORCounterConfigField* counterConfig,
                                       int nReason) override;
#ifdef USE_OPENORDERAPI
    virtual void onLogin(or_channelid_i channelId, ORRspLoginField* pRspLogin,
                         ORRspInfoField* pRspInfo) override;
    virtual void onRuntimeStatus(or_channelid_i channelId,
                                 ORRuntimeStatusField* pStatus,
                                 ORRspInfoField* pRspInfo) override;
    virtual void onCheckAccount(or_channelid_i channelId,
                                ORCheckAccountField* pInvestor,
                                ORRspInfoField* pRspInfo) override{};
    virtual void onAssetTransfer(or_channelid_i channelId,
                                 ORAssetTransferField* pTransfer,
                                 ORRspInfoField* pRspInfo) override
    {
    }
    virtual void onError(or_channelid_i channelId,
                         ORRspInfoField* pRspInfo) override{};
    virtual void onRtnOrder(or_channelid_i channelId,
                            OROrderField* pOrder) override;
    virtual void onRtnTrade(or_channelid_i channelId,
                            ORTradeField* pTrade) override;
    virtual void onErrRtnOrderSend(or_channelid_i channelId,
                                   OROrderReqField* pInputOrder,
                                   ORRspInfoField* pRspInfo) override;
    virtual void onErrRtnOrderCancel(or_channelid_i channelId,
                                     OROrderCancelReqField* pOrderAction,
                                     ORRspInfoField* pRspInfo) override;
    virtual void onRspTradeDate(or_channelid_i channelId,
                                ORRspTradeDateField* pRspTradeDate,
                                ORRspInfoField* pRspInfo) override{};
    virtual void onRspStockCode(or_channelid_i channelId,
                                ORRspStockCodeField* pRspStockCode,
                                ORRspInfoField* pRspInfo) override
    {
    }
    virtual void onRspQryTradeCode(or_channelid_i channelId,
                                   ORTradingCodeField* pRspTradingCode,
                                   ORRspInfoField* pRspInfo) override{};
    virtual void onRspQryTradingCodeType(or_channelid_i channelId,
                                         ORTradingCodeField* pRspTradingCode,
                                         ORRspInfoField* pRspInfo) override{};
    virtual void
    onRspQryFuturesAccountMargin(or_channelid_i channelId,
                                 ORAccountAssetField* pRspFuturesAccountMargin,
                                 ORRspInfoField* pRspInfo) override{};
    virtual void onRspQryStockPurQuotation(or_channelid_i channelId,
                                           ORStockPurQuotationField* field,
                                           ORRspInfoField* pRspInfo) override{};
    virtual void onRspQryStockPurInfo(or_channelid_i channelId,
                                      ORStockPurInfoField* field,
                                      ORRspInfoField* pRspInfo) override{};
    virtual void
    onRspQryStockPurStatistics(or_channelid_i channelId,
                               ORStockPurStatisticsField* field,
                               ORRspInfoField* pRspInfo) override{};
    virtual void onRspQryAccountAsset(or_channelid_i channelId,
                                      ORAccountAssetField* pRspAccountAsset,
                                      ORRspInfoField* pRspInfo) override;
    virtual void onRspQryPosition(or_channelid_i channelId,
                                  ORAccountPositionField* pRspPosition,
                                  ORRspInfoField* pRspInfo) override;
    virtual void onRspQryPositionDtl(or_channelid_i channelId,
                                     ORAccountPositionDtlField* pRspPositionDtl,
                                     ORRspInfoField* pRspInfo) override;
    virtual void onRspQryMarginRate(or_channelid_i channelId,
                                    ORMarginRateField* pRspMarginRate,
                                    ORRspInfoField* pRspInfo) override{};
    virtual void
    onDepthMarketData(or_channelid_i channelId,
                      ORDepthMarketDataField* pDepthMarketData) override;
#else
    virtual void onLogin(ORRspLoginField* pRspLogin,
                         ORRspInfoField* pRspInfo) override;
    virtual void onRuntimeStatus(ORRuntimeStatusField* pStatus,
                                 ORRspInfoField* pRspInfo) override;
    virtual void onCheckAccount(ORCheckAccountField* pInvestor,
                                ORRspInfoField* pRspInfo) override{};
    virtual void onAssetTransfer(ORAssetTransferField* pTransfer,
                                 ORRspInfoField* pRspInfo) override
    {
    }
    virtual void onError(ORRspInfoField* pRspInfo) override{};
    virtual void onRtnOrder(OROrderField* pOrder) override;
    virtual void onRtnTrade(ORTradeField* pTrade) override;
    virtual void onErrRtnOrderSend(OROrderReqField* pInputOrder,
                                   ORRspInfoField* pRspInfo) override;
    virtual void onErrRtnOrderCancel(OROrderCancelReqField* pOrderAction,
                                     ORRspInfoField* pRspInfo) override;
    virtual void onRspTradeDate(ORRspTradeDateField* pRspTradeDate,
                                ORRspInfoField* pRspInfo) override{};
    virtual void onRspStockCode(ORRspStockCodeField* pRspStockCode,
                                ORRspInfoField* pRspInfo) override
    {
    }
    virtual void onRspQryTradeCode(ORTradingCodeField* pRspTradingCode,
                                   ORRspInfoField* pRspInfo) override{};
    virtual void onRspQryTradingCodeType(ORTradingCodeField* pRspTradingCode,
                                         ORRspInfoField* pRspInfo) override{};
    virtual void
    onRspQryFuturesAccountMargin(ORAccountAssetField* pRspFuturesAccountMargin,
                                 ORRspInfoField* pRspInfo) override{};
    virtual void onRspQryStockPurQuotation(ORStockPurQuotationField* field,
                                           ORRspInfoField* pRspInfo) override{};
    virtual void onRspQryStockPurInfo(ORStockPurInfoField* field,
                                      ORRspInfoField* pRspInfo) override{};
    virtual void
    onRspQryStockPurStatistics(ORStockPurStatisticsField* field,
                               ORRspInfoField* pRspInfo) override{};
    virtual void onRspQryAccountAsset(ORAccountAssetField* pRspAccountAsset,
                                      ORRspInfoField* pRspInfo) override;
    virtual void onRspQryPosition(ORAccountPositionField* pRspPosition,
                                  ORRspInfoField* pRspInfo) override;
    virtual void onRspQryPositionDtl(ORAccountPositionDtlField* pRspPositionDtl,
                                     ORRspInfoField* pRspInfo) override;
    virtual void onRspQryMarginRate(ORMarginRateField* pRspMarginRate,
                                    ORRspInfoField* pRspInfo) override{};
    virtual void
    onDepthMarketData(or_channelid_i channelId,
                      ORDepthMarketDataField* pDepthMarketData) override;
#endif

private:
#ifdef USE_OPENORDERAPI
#else
    or_channelid_i channel_id_;
#endif
    ChannelHooked* hook_;
};
using ChannelSpiPtr = std::shared_ptr<ChannelSpi>;

} // namespace router
} // namespace framework