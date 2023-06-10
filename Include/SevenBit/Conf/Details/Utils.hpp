#pragma once

#include <charconv>
#include <cstddef>
#include <exception>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Exceptions.hpp"

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

    template <class TNumber> std::pair<bool, TNumber> tryStringTo(std::string_view str, bool full = true)
    {
        TNumber number = 0;
        while (!str.empty() && std::isspace(str.front()))
        {
            str.remove_prefix(1);
        }
        auto last = str.data() + str.size();
        auto res = std::from_chars(str.data(), last, number);
        auto success = res.ec == std::errc{} && (!full || res.ptr == last);
        return {success, number};
    }

    template <> inline std::pair<bool, double> tryStringTo<double>(std::string_view str, bool full)
    {
        try
        {
            std::string string{str};
            size_t processed = 0;
            auto number = std::stod(string, &processed);
            auto success = !full || processed == string.size();
            return {success, number};
        }
        catch (std::exception &e)
        {
            return {false, 0.0};
        }
    }

    template <> inline std::pair<bool, bool> tryStringTo<bool>(std::string_view str, bool full)
    {
        if (ignoreCaseEquals("true", str))
        {
            return {true, true};
        }
        else if (ignoreCaseEquals("false", str))
        {
            return {true, false};
        }
        else if (auto [success, number] = tryStringTo<int>(str, full); success)
        {
            return {true, number};
        }
        return {false, false};
    }

    template <class TNumber> TNumber stringTo(std::string_view str, bool full = true)
    {
        if (auto [success, number] = tryStringTo<TNumber>(str, full); success)
        {
            return number;
        }
        throw ConfigException{"Cannot convert string to number" + std::string{str}};
    }
} // namespace sb::cf::details::utils

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/Utils.hpp"
#endif