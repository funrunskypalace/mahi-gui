#pragma once

namespace framework
{
namespace trade
{

/// 对接交易API的一个通道
class Channel
{
public:
    virtual ~Channel() = default;

    virtual int start() PURE;
};

} // namespace trade
} // namespace framework