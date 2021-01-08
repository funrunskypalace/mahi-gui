#pragma once

#include "fmt/format.h"

#include "framework/common/common.h"

namespace framework{
    
class MarketPluginId
{
public:
    MarketPluginId() {}
    MarketPluginId(const std::string& value) : value_(value) {}
    MarketPluginId(const char* value) : value_(value) {}
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

class MarketPluginInstId
{
public:
    MarketPluginInstId() {}
    MarketPluginInstId(const std::string& value) : value_(value) {}
    MarketPluginInstId(const char* value) : value_(value) {}
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
struct formatter<framework::MarketPluginId> : fmt::formatter<std::string>
{
    auto format(const framework::MarketPluginId& s, fmt::format_context& ctx)
    {
        return fmt::formatter<std::string>::format(s.str_value(), ctx);
    }
};

template <>
struct formatter<framework::MarketPluginInstId> : fmt::formatter<std::string>
{
    auto format(const framework::MarketPluginInstId& s,
                fmt::format_context& ctx)
    {
        return fmt::formatter<std::string>::format(s.str_value(), ctx);
    }
};

} // namespace v6
} // namespace fmt

namespace std
{
ostream& operator<<(ostream& os, const framework::MarketPluginId& v);
ostream& operator<<(ostream& os, const framework::MarketPluginInstId& v);
istream& operator>>(istream& is, framework::MarketPluginId& v);
istream& operator>>(istream& is, framework::MarketPluginInstId& v);

template <> struct hash<framework::MarketPluginId>
{
    std::size_t operator()(const framework::MarketPluginId& k) const
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
template <> struct hash<framework::MarketPluginInstId>
{
    std::size_t operator()(const framework::MarketPluginInstId& k) const
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
template <> struct TypeDesc<framework::MarketPluginId>
{
    static const char* Name() { return "<framework::MarketPluginId>"; }
};
template <> struct TypeDesc<framework::MarketPluginInstId>
{
    static const char* Name() { return "<framework::MarketPluginInstId>"; }
};
} // namespace cli
