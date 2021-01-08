#pragma once

namespace framework
{

typedef int error_type;
const int F_NOERR = 0;
const int F_FAIL = -1;
const int F_TRANSACTION_CONFLICT = -5000;
const int F_TRANSACTION_FINISHED = 5001;

// Strategy-related errors
const int F_STRATEGY_PARAMS_NOT_VALID = -9001;

} // namespace framework