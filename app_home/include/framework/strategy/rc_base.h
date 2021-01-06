#pragma once

#include "framework/common/common.h"
#include "framework/trade/transaction.h"

namespace framework
{
namespace strat
{

/// Risk Control base class.
class RcBase
{
public:
    virtual bool canTerminate(int orderIndex) const PURE;
    virtual bool riskCheck(const trade::Transaction& tran) PURE;
};
using RcBasePtr = std::shared_ptr<RcBase>;
} // namespace strat
} // namespace framework
