// @author Jiang Sanyi 2019-12-03
#pragma once

#include "framework/common/common.h"
#include "framework/market/market.h"

namespace framework
{
namespace market
{

/**
 * ϵͳ�ڽ�֧�ֵ����鴦������ͨ�����ô������Խ�������Դ��ת����
 */
class MarketHandler
{
public:
    /// callback���ڽ��յ�����֮�󣬽�����Ͷ�ݵ���ܡ�
    virtual int initialize(MarketPostCb callback) PURE;

    /// �ر�������Դ�����ӡ�
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
