#pragma once

#include <cstddef>
#include <exception>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/Exceptions.hpp"

namespace sb::cf::details::utils
{
    EXPORT bool ignoreCaseEquals(std::string_view stra, std::string_view strb);

    EXPORT bool startsWith(std::string_view str, std::string_view search, bool ignoreCase = false);

    EXPORT bool endsWith(std::string_view str, std::string_view search, bool ignoreCase = false);

    EXPORT std::size_t replaceAll(std::string &inout, std::string_view what, std::string_view with);

    EXPORT std::vector<std::string_view> split(std::string_view str, const std::string_view &delim = "/",
                                               size_t max = std::numeric_limits<size_t>::max());

    EXPORT std::string joinViews(const std::vector<std::string_view> &strs, const std::string &divider);

    EXPORT std::string join(const std::vector<std::string> &strs, const std::string &divider);

    EXPORT bool isNumberString(std::string_view str);

    template <class T> struct UnsupportedType : std::false_type
    {
    };

    template <class TNumber> TNumber convertTo(std::string_view str, size_t &processed)
    {
        if constexpr (std::is_same_v<TNumber, double>)
        {
            return std::stod(std::string{str}, &processed);
        }
        else if constexpr (std::is_same_v<TNumber, int>)
        {
            return std::stoi(std::string{str}, &processed);
        }
        else if constexpr (std::is_same_v<TNumber, std::int64_t>)
        {
            return std::stoll(std::string{str}, &processed);
        }
        else if constexpr (std::is_same_v<TNumber, size_t>)
        {
            return std::stoul(std::string{str}, &processed);
        }
        else if constexpr (std::is_same_v<TNumber, std::uint64_t>)
        {
            return std::stoull(std::string{str}, &processed);
        }
        else
        {
            static_assert(UnsupportedType<TNumber>::value, "unsupported type");
        }
    }

    template <class TNumber> TNumber stringTo(std::string_view str, bool full = true)
    {
        try
        {
            size_t processed = 0;
            auto number = convertTo<TNumber>(str, processed);
            if (full && processed != str.size())
            {
                throw ConfigException{"Cannot convert string '" + std::string{str} +
                                      "' to number, not all characters were converted"};
            }
            return number;
        }
        catch (std::exception &e)
        {
            throw ConfigException{"Cannot convert string '" + std::string{str} + "' to number, reason: " + e.what()};
        }
    }

    template <> inline bool stringTo<bool>(std::string_view str, bool full)
    {
        if (ignoreCaseEquals("true", str))
        {
            return true;
        }
        else if (ignoreCaseEquals("false", str))
        {
            return false;
        }
        return stringTo<int>(str, full);
    }

    template <class TNumber> std::pair<bool, TNumber> tryStringTo(std::string_view str, bool full = true)
    {
        try
        {
            return {true, stringTo<TNumber>(str, full)};
        }
        catch (std::exception &e)
        {
            return {false, 0};
        }
    }
} // namespace sb::cf::details::utils

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/Utils.hpp"
#endif