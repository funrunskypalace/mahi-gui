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

/// 设计思路：
/// 1.
/// 原先将TradeEngine作为OrderRouterSpi派生类，但是这样丢失了
/// 通道回报的ChannelID信息。框架不能这么做，所以另外设定了ChannelSpi类
/// 作为SPI，然后集成在TradeEngine中。
/// 2.
/// TradeEngine仅作为报文中转通道（管理），不进行委托状态管理。
/// 各个Context应该维护各自账号与用户操作账号的委托状态和持仓状态。
/// 3.
/// TradeEngine负责实现事务Transaction的持久化流文件管理。
/// 应该是基于OrderPackInfo重演，还是CBizPackage来重演？这是个问题。
/// 基于前者可以兼容目前代码实现方式，TradeEngine中的缓存状态可以恢复。
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