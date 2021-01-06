// @author Jiang Sanyi 2019-12-03
#pragma once

#include "framework/common/common.h"
#include "framework/market/market.h"

namespace framework
{
namespace market
{

/**
 * 系统内建支持的行情处理器。通过配置创建，对接行情来源并转发。
 */
class MarketHandler
{
public:
    /// callback用于接收到行情之后，将行情投递到框架。
    virtual int initialize(MarketPostCb callback) PURE;

    /// 关闭与行情源的连接。
    virtual void close() PURE;

    virtual bool isExiting() PURE;

    /// keep subscribtion to a specified instrument
    virtual void subscribe(int dataType, const std::string& exId,
                           const std::string& insId){};

    virtual void unsubscribe(int dataType, const std::string& exId,
                             const std::string& insId){};

    virtual ~MarketHandler(){};

protected:
    virtual void onRtnDepthMarketData(int32_t dataType, const char* data,
                                      size_t datasiz) PURE;
};

using MarketHandlerPtr = std::shared_ptr<MarketHandler>;

} // namespace market
} // namespace framework
