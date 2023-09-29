#pragma once

#include <charconv>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details::utils
{
    EXPORT [[nodiscard]] bool isNumberString(std::string_view str);

    EXPORT [[nodiscard]] bool ignoreCaseLess(std::string_view str, std::string_view search);

    EXPORT [[nodiscard]] bool ignoreCaseEqual(std::string_view str, std::string_view search);

    EXPORT [[nodiscard]] bool containsAt(std::string_view str, size_t index, std::string_view search);

    EXPORT [[nodiscard]] std::optional<std::string_view> containsAt(std::string_view str, size_t index,
                                                                    const std::vector<std::string_view> &searches);

    EXPORT [[nodiscard]] bool containsAtFromEnd(std::string_view str, size_t index, std::string_view search);

    EXPORT [[nodiscard]] std::optional<std::string_view> containsAtFromEnd(
        std::string_view str, size_t index, const std::vector<std::string_view> &searches);

    EXPORT [[nodiscard]] bool startsWith(std::string_view str, std::string_view search);

    EXPORT [[nodiscard]] std::vector<std::string_view> split(std::string_view str, std::string_view divider);

    EXPORT [[nodiscard]] std::vector<std::string_view> split(std::string_view str,
                                                             const std::vector<std::string_view> &dividers);

    EXPORT [[nodiscard]] std::optional<std::pair<std::string_view, std::string_view>> tryBreak(
        std::string_view str, const std::vector<std::string_view> &dividers);

    EXPORT [[nodiscard]] std::optional<std::pair<std::string_view, std::string_view>> tryBreakFromEnd(
        std::string_view str, const std::vector<std::string_view> &dividers);

    EXPORT [[nodiscard]] std::string joinViews(const std::vector<std::string_view> &strings,
                                               const std::string &divider);

    template <class T> void assertPtr(const T *ptr)
    {
        if (!ptr)
        {
            throw NullPointerException(std::string{"Pointer of type: '"} + typeid(T).name() + "' cannot be null");
        }
    }

    template <class T> void assertPtr(const std::unique_ptr<T> &ptr) { assertPtr(ptr.get()); }

    template <class T> void assertPtr(const std::shared_ptr<T> &ptr) { assertPtr(ptr.get()); }

    template <class TNumber> [[nodiscard]] std::pair<bool, TNumber> tryStringTo(std::string_view str, bool full = true)
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

    template <> [[nodiscard]] inline std::pair<bool, double> tryStringTo<double>(std::string_view str, bool full)
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

    template <> [[nodiscard]] inline std::pair<bool, bool> tryStringTo<bool>(std::string_view str, bool full)
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

    template <class TNumber> [[nodiscard]] TNumber stringTo(std::string_view str, bool full = true)
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
