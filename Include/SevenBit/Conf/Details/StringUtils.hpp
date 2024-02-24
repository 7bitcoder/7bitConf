#pragma once

#include <charconv>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    struct EXPORT StringUtils
    {
        static bool isNumber(std::string_view str);

        static bool ignoreCaseLess(std::string_view str, std::string_view search);

        static bool ignoreCaseEqual(std::string_view str, std::string_view search);

        static bool containsAt(std::string_view str, size_t index, std::string_view search);

        static std::optional<std::string_view> containsAt(std::string_view str, size_t index,
                                                          const std::vector<std::string_view> &searches);

        static bool containsAtFromEnd(std::string_view str, size_t index, std::string_view search);

        static std::optional<std::string_view> containsAtFromEnd(std::string_view str, size_t index,
                                                                 const std::vector<std::string_view> &searches);

        static bool startsWith(std::string_view str, std::string_view search);

        static size_t startsWithWhiteSpace(std::string_view str);

        static bool isWhiteSpace(std::string_view str);

        static std::vector<std::string_view> split(std::string_view str, std::string_view separator);

        static std::vector<std::string_view> split(std::string_view str,
                                                   const std::vector<std::string_view> &separators);

        static std::optional<std::pair<std::string_view, std::string_view>> tryBreak(
            std::string_view str, const std::vector<std::string_view> &separators);

        static std::optional<std::pair<std::string_view, std::string_view>> tryBreakFromEnd(
            std::string_view str, const std::vector<std::string_view> &separators);

        static std::string join(const std::vector<std::string_view> &strs, const std::string &separator);

        template <class TNumber>
        static std::pair<bool, TNumber> tryConvertTo(std::string_view str, const bool full = true)
        {
            TNumber number = 0;
            str.remove_prefix(startsWithWhiteSpace(str));
            auto last = str.data() + str.size();
            auto res = std::from_chars(str.data(), last, number);
            auto success = res.ec == std::errc{} && (!full || res.ptr == last);
            return {success, number};
        }

        template <class TNumber> static TNumber convertTo(std::string_view str, const bool full = true)
        {
            if (auto [success, number] = tryConvertTo<TNumber>(str, full); success)
            {
                return number;
            }
            throw ConfigException{"Cannot convert string to number: " + std::string{str}};
        }
    };

    template <> inline std::pair<bool, double> StringUtils::tryConvertTo<double>(std::string_view str, const bool full)
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

    template <> inline std::pair<bool, bool> StringUtils::tryConvertTo<bool>(std::string_view str, const bool full)
    {
        if (ignoreCaseEqual(str, "true"))
        {
            return {true, true};
        }
        else if (ignoreCaseEqual(str, "false"))
        {
            return {true, false};
        }
        else if (auto [success, number] = tryConvertTo<int>(str, full); success)
        {
            return {true, number};
        }
        return {false, false};
    }
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/StringUtils.hpp"
#endif
