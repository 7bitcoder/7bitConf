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
    EXPORT bool ignoreCaseEqual(char cha, char chb);

    EXPORT bool ignoreCaseEqual(std::string_view str, std::string_view search);

    EXPORT std::optional<std::string_view> containsAt(std::string_view str, size_t index,
                                                      const std::vector<std::string_view> &searches);

    EXPORT bool containsAt(std::string_view str, size_t index, std::string_view search);

    EXPORT std::optional<std::string_view> backwardContainsAt(std::string_view str, size_t index,
                                                              const std::vector<std::string_view> &searches);

    EXPORT bool backwardContainsAt(std::string_view str, size_t index, std::string_view search);

    EXPORT bool ignoreCaseContainsAt(std::string_view str, size_t index, std::string_view search);

    EXPORT bool startsWith(std::string_view str, std::string_view search);

    EXPORT bool ignoreCaseStartsWith(std::string_view str, std::string_view search);

    EXPORT bool endsWith(std::string_view str, std::string_view search);

    EXPORT bool ignoreCaseEndsWith(std::string_view str, std::string_view search);

    EXPORT std::size_t replaceAll(std::string &inout, std::string_view what, std::string_view with);

    EXPORT std::vector<std::string_view> split(std::string_view str, const std::string_view &delim = "/",
                                               size_t max = std::numeric_limits<size_t>::max());

    EXPORT std::vector<std::string_view> split(std::string_view str, const std::vector<std::string_view> &delims,
                                               size_t max = std::numeric_limits<size_t>::max());
    
    EXPORT std::string joinViews(const std::vector<std::string_view> &strings, const std::string &divider);

    EXPORT std::string join(const std::vector<std::string> &strings, const std::string &divider);

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
        if (ignoreCaseEqual(str, "true"))
        {
            return {true, true};
        }
        else if (ignoreCaseEqual(str, "false"))
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
        throw ConfigException{"Cannot convert string to number: " + std::string{str}};
    }
} // namespace sb::cf::details::utils

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/Utils.hpp"
#endif