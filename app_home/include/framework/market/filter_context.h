#pragma once

#include "framework/common/logger.h"

namespace framework
{
namespace market
{

/// 相对于runtime::Context来说是一个简单版本。
/// 主要参考后者的封装思想：传入Strategy实例的是基类Context，但是
/// 背后有一个ContextImpl实现多项封装。所以我们传给行情增强Filter
/// 实例的是FilterContext基类，但是背后有一个FilterContextImpl实现了
/// 订阅上下文的封装。而FilterContext基类则只需要提供Plugin所需要的
/// 最简单的接口即可。
class FilterContext
{
public:
    virtual ~FilterContext() = default;

    virtual std::string uuid() const PURE;

    template <typename... Args>
    void log(int level, const char* fmt, const Args&... args)
    {
        if (nullptr == logger())
            return;
        logger()->log(level, fmt, args...);
    }

    virtual GreatWallLoggerPtr logger() const PURE;

    /// 过滤器下游对象（动态插件）订阅所需要的行情
    virtual void subscribe(int dataType, const std::string& exId,
                           const std::string& insId) PURE;

    /// 过滤器下游对象（动态插件）取消订阅行情
    virtual void unsubscribe(int dataType, const std::string& exId,
                             const std::string& insId) PURE;

    virtual void stop() PURE;

    virtual void feedBack(int dataType, const char* data, size_t datasiz) PURE;
};
using FilterContextPtr = std::shared_ptr<FilterContext>;

} // namespace market
} // namespace framework
