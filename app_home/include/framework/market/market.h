// @author Jiang Sanyi 2019-12-03
#pragma once

#include "framework/common/common.h"

namespace framework
{
namespace market
{

/// 要避免在代码中 使用回调对象的引用进行对象内存储。
using MarketPostCb =
    std::function<void(int32_t dataType, const char* data, size_t datasiz)>;

/// 要避免在代码中 使用回调对象的引用进行对象内存储。
using MarketPluginBackCb =
    std::function<void(int32_t dataType, const char* data, size_t datasiz)>;

} // namespace market
} // namespace framework
