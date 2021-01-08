#pragma once

#include "framework/common/common.h"
#include "framework/common/logger.h"

#include "absl/time/clock.h"
#include "absl/time/time.h"

namespace framework
{
namespace time
{

/// 每个策略会有一个专属的Context。
/// 每个Context可以同时处理多个事务。策略应自行限制事务。
class GWSDK_API ClockUtil
{
public:
    ClockUtil() = default;
    absl::Time startWatch() const;
    absl::Duration stopWatch(const absl::Time& start) const;
    absl::Duration stopWatch(const absl::Time& start, absl::Time& end) const;
};
using ClockUtilPtr = std::unique_ptr<ClockUtil>;

} // namespace time
} // namespace framework
