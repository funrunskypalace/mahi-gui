#pragma once

namespace framework
{
namespace trade
{

/// �Խӽ���API��һ��ͨ��
class Channel
{
public:
    virtual ~Channel() = default;

    virtual int start() PURE;
};

} // namespace trade
} // namespace framework