#pragma once

#include "framework/common/common.h"
#include "framework/strategy/instance.h"

namespace framework
{
namespace strat
{

class StrategyEngine
{
public:
    virtual ~StrategyEngine() = default;

    virtual int start() PURE;

    /// synchronous stop and return, no need to join.
    virtual int stop() PURE;

    /// dynamic create strategy
    /// @param strategyId strategy-id
    /// @param strategyInstId newly created strategy's instance id.
    virtual int load(const StrategyId& strategyId,
                     const StrategyInstId& strategyInstId) PURE;

    virtual int unload(const StrategyId& strategyId,
                       const StrategyInstId& strategyInstId,
                       std::string& reason) PURE;

    virtual InstancePtr getInstance(const StrategyInstId& strategyInstId) PURE;

    virtual std::list<strat::InstancePtr>
    getInstances(const StrategyId& strategyId) PURE;
};

} // namespace strat
} // namespace framework