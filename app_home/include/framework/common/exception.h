#pragma once

#include <stdexcept>
#include <string>

namespace framework
{
class GreatWallException : public std::runtime_error
{
public:
    GreatWallException(const std::string& message) : std::runtime_error(message)
    {
    }
};
} // namespace framework