#pragma once

#include <charconv>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf::utils
{
    EXPORT bool ignoreCaseEquals(std::string_view stra, std::string_view strb);

    EXPORT bool startsWith(std::string_view str, std::string_view search);

    EXPORT bool endsWith(std::string_view str, std::string_view search);

    EXPORT std::size_t replaceAll(std::string &inout, std::string_view what, std::string_view with);

    EXPORT std::vector<std::string_view> split(std::string_view str, const std::string_view &delim = "/",
                                               size_t max = std::numeric_limits<size_t>::max());

    EXPORT std::string join(const std::vector<std::string_view> &strs, const std::string &divider);

    EXPORT bool isNumberString(std::string_view str);

    template <class TNumber> std::pair<bool, TNumber> toNumber(std::string_view str, bool full = true, int base = 10)
    {
        TNumber number = 0;
        auto last = str.begin() + str.size();
        auto res = std::from_chars(str.begin(), last, number, base);
        auto success = res.ec == std::errc{} && (!full || res.ptr == last);
        return {success, number};
    }
} // namespace sb::cf::utils

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/Utils.hpp"
#endif