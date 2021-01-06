#pragma once

#include "framework/application/service.h"
#include "framework/common/common.h"
#include "biz/position/PosCache.h"
#include "framework/router/channel_spi.h"

using namespace cycnoches::biz;

namespace framework
{
namespace router
{

struct PersistentInfo
{
    int32_t LastDate;
    or_orrequestid_i ORRequestID;
    int64_t Reserved1;
    int64_t Reserved2;
    int64_t Reserved3;
    int64_t Reserved4;
};

/// ���˼·��
/// 1.
/// ԭ�Ƚ�TradeEngine��ΪOrderRouterSpi�����࣬����������ʧ��
/// ͨ���ر���ChannelID��Ϣ����ܲ�����ô�������������趨��ChannelSpi��
/// ��ΪSPI��Ȼ�󼯳���TradeEngine�С�
/// 2.
/// TradeEngine����Ϊ������תͨ����������������ί��״̬����
/// ����ContextӦ��ά�������˺����û������˺ŵ�ί��״̬�ͳֲ�״̬��
/// 3.
/// TradeEngine����ʵ������Transaction�ĳ־û����ļ�����
/// Ӧ���ǻ���OrderPackInfo���ݣ�����CBizPackage�����ݣ����Ǹ����⡣
/// ����ǰ�߿��Լ���Ŀǰ����ʵ�ַ�ʽ��TradeEngine�еĻ���״̬���Իָ���
class TradeEngineBase : public ChannelHooked
{
public:
    TradeEngineBase(const GreatWallConfigPtr& configs);
    virtual ~TradeEngineBase();
    int start(RunMode runMode, const std::string& appId);
    bool canStop() const;
    int stop();
    const PositionCachePtr positionCache() const { return position_cache_; };
    const OrderCachePtr orderCache() const { return order_cache_; };
    const GreatWallConfigPtr& appConfigs() const;
    OrderRouterApi* getChannel(or_channelid_i channelId) const;

public: // ChannelHooked
#ifdef USE_OPENORDERAPI
    virtual void onConnected();
#endif

private:
    virtual void onPositionChanged(or_channelid_i channelId,
                                   ORAccountPositionField* pRspPosition,
                                   ORRspInfoField* pRspInfo) override;
    virtual void onAccountAssetChanged(or_channelid_i channelId,
                                       ORAccountAssetField* pRspPosition,
                                       ORRspInfoField* pRspInfo) override;

protected:
#ifdef USE_OPENORDERAPI
    int initializeOos();
#endif

    virtual void notifyTradeEngineStatus(const std::string& statusCode,
                                         const std::string& statusMsg){};
    virtual int onPreStart(or_channelid_i channelId) { return F_NOERR; };
    virtual void onInitializeInvestor(or_investorid_i investorId){};
    virtual int onPreStop(or_channelid_i channelId) { return F_NOERR; };

private:
    std::atomic_bool is_initializing_;
    std::atomic_bool is_exited_;
    std::atomic_bool is_exiting_;
    GreatWallConfigPtr app_configs_;
    PositionCachePtr position_cache_;
    OrderCachePtr order_cache_;

#ifdef USE_OPENORDERAPI
    OpenOrderApi* oos_api_ = nullptr;
#else
    std::list<ChannelSpiPtr> channel_spis_;
#endif
};

} // namespace router
} // namespace framework