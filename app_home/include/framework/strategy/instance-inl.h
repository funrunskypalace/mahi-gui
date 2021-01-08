#pragma once

#include "fmt/format.h"

#include "framework/common/common.h"

namespace framework
{
    
class StrategyId
{
public:
    StrategyId() {}
    StrategyId(const std::string& value) : value_(value) {}
    StrategyId(const char* value) : value_(value) {}
    operator std::string() const { return value_; }
    std::string str_value() const { return value_; }
    void str_value(const std::string& value) { value_ = value; }
    bool empty() const { return value_.empty(); }
    const char* c_str() const { return value_.c_str(); }
    bool operator==(const std::string& other) const { return other == value_; }
    bool operator==(const char* other) const { return other == value_; }
    size_t length() const { return value_.length(); }

private:
    std::string value_;
};

class StrategyInstId
{
public:
    StrategyInstId() {}
    StrategyInstId(const std::string& value) : value_(value) {}
    StrategyInstId(const char* value) : value_(value) {}
    operator std::string() const { return value_; }
    std::string str_value() const { return value_; }
    void str_value(const std::string& value) { value_ = value; }
    bool empty() const { return value_.empty(); }
    const char* c_str() const { return value_.c_str(); }
    bool operator==(const std::string& other) const { return other == value_; }
    bool operator==(const char* other) const { return other == value_; }
    size_t length() const { return value_.length(); }

private:
    std::string value_;
};
} // namespace framework

namespace fmt
{
namespace v6
{

template <>
struct formatter<framework::StrategyId> : fmt::formatter<std::string>
{
    auto format(const framework::StrategyId& s, fmt::format_context& ctx)
    {
        return fmt::formatter<std::string>::format(s.str_value(), ctx);
    }
};

template <>
struct formatter<framework::StrategyInstId> : fmt::formatter<std::string>
{
    auto format(const framework::StrategyInstId& s, fmt::format_context& ctx)
    {
        return fmt::formatter<std::string>::format(s.str_value(), ctx);
    }
};

} // namespace v6
} // namespace fmt

namespace std
{
ostream& operator<<(ostream& os,
                              const framework::StrategyId& v);
ostream& operator<<(ostream& os,
                              const framework::StrategyInstId& v);
istream& operator>>(istream& is, framework::StrategyId& v);
istream& operator>>(istream& is, framework::StrategyInstId& v);

template <> struct hash<framework::StrategyId>
{
    std::size_t operator()(const framework::StrategyId& k) const
    {
        using std::hash;
        using std::size_t;
        using std::string;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:

        return hash<string>()(k.str_value());
    }
};
template <> struct hash<framework::StrategyInstId>
{
    std::size_t operator()(const framework::StrategyInstId& k) const
    {
        using std::hash;
        using std::size_t;
        using std::string;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:

        return hash<string>()(k.str_value());
    }
};

} // namespace std

#include "cli/cli.h"

namespace cli
{
template <> struct TypeDesc<framework::StrategyId>
{
    static const char* Name() { return "<framework::StrategyId>"; }
};
template <> struct TypeDesc<framework::StrategyInstId>
{
    static const char* Name() { return "<framework::StrategyInstId>"; }
};
} // namespace cli
