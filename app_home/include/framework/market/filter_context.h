#pragma once

#include "framework/common/logger.h"

namespace framework
{
namespace market
{

/// �����runtime::Context��˵��һ���򵥰汾��
/// ��Ҫ�ο����ߵķ�װ˼�룺����Strategyʵ�����ǻ���Context������
/// ������һ��ContextImplʵ�ֶ����װ���������Ǵ���������ǿFilter
/// ʵ������FilterContext���࣬���Ǳ�����һ��FilterContextImplʵ����
/// ���������ĵķ�װ����FilterContext������ֻ��Ҫ�ṩPlugin����Ҫ��
/// ��򵥵Ľӿڼ��ɡ�
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

    /// ���������ζ��󣨶�̬�������������Ҫ������
    virtual void subscribe(int dataType, const std::string& exId,
                           const std::string& insId) PURE;

    /// ���������ζ��󣨶�̬�����ȡ����������
    virtual void unsubscribe(int dataType, const std::string& exId,
                             const std::string& insId) PURE;

    virtual void stop() PURE;

    virtual void feedBack(int dataType, const char* data, size_t datasiz) PURE;
};
using FilterContextPtr = std::shared_ptr<FilterContext>;

} // namespace market
} // namespace framework
