// @author Jiang Sanyi 2019-12-03
#pragma once

#include "framework/common/common.h"

namespace framework
{
namespace market
{

/// Ҫ�����ڴ����� ʹ�ûص���������ý��ж����ڴ洢��
using MarketPostCb =
    std::function<void(int32_t dataType, const char* data, size_t datasiz)>;

/// Ҫ�����ڴ����� ʹ�ûص���������ý��ж����ڴ洢��
using MarketPluginBackCb =
    std::function<void(int32_t dataType, const char* data, size_t datasiz)>;

} // namespace market
} // namespace framework
