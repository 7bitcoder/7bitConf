#pragma once

#include <algorithm>
#include <cctype>

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details::utils
{
    INLINE bool isNumberString(std::string_view str)
    {
        return !str.empty() && std::all_of(str.begin(), str.end(), [](char ch) { return std::isdigit(ch); });
    }

    INLINE bool ignoreCaseLess(std::string_view str, std::string_view search)
    {
        return std::lexicographical_compare(str.begin(), str.end(), search.begin(), search.end(),
                                            [](char cha, char chb) { return std::tolower(cha) < std::tolower(chb); });
    }

    INLINE bool ignoreCaseEqual(std::string_view str, std::string_view search)
    {
        return str.size() == search.size() &&
               std::equal(str.begin(), str.end(), search.begin(), search.end(),
                          [](char cha, char chb) { return std::tolower(cha) == std::tolower(chb); });
    }

    INLINE bool containsAt(std::string_view str, size_t index, std::string_view search)
    {
        if (index >= str.size() || search.empty())
        {
            return false;
        }
        return str.compare(index, search.size(), search) == 0;
    }

    INLINE std::optional<std::string_view> containsAt(std::string_view str, size_t index,
                                                      const std::vector<std::string_view> &searches)
    {
        for (auto &search : searches)
        {
            if (containsAt(str, index, search))
            {
                return search;
            }
        }
        return std::nullopt;
    }

    INLINE bool containsAtFromEnd(std::string_view str, size_t index, std::string_view search)
    {
        if (index + 1 < search.size())
        {
            return false;
        }
        return containsAt(str, index + 1 - search.size(), search);
    }

    INLINE std::optional<std::string_view> containsAtFromEnd(std::string_view str, size_t index,
                                                             const std::vector<std::string_view> &searches)
    {
        for (auto &search : searches)
        {
            if (containsAtFromEnd(str, index, search))
            {
                return search;
            }
        }
        return std::nullopt;
    }

    INLINE bool startsWith(std::string_view str, std::string_view search)
    {
        auto searchIt = search.begin();
        for (auto it = str.begin(); it != str.end() && searchIt != search.end(); ++it, ++searchIt)
        {
            if (*it != *searchIt)
            {
                return false;
            }
        }
        return searchIt == search.end();
    }

    INLINE std::vector<std::string_view> split(std::string_view str, std::string_view divider)
    {
        std::vector<std::string_view> result;
        std::string::size_type begin = 0, pos = 0;
        for (; std::string_view::npos != (pos = str.find_first_of(divider, pos)); begin = (pos += divider.size()))
        {
            result.push_back(str.substr(begin, pos - begin));
        }
        result.push_back(str.substr(begin));
        return result;
    }

    INLINE std::vector<std::string_view> split(std::string_view str, const std::vector<std::string_view> &dividers)
    {
        std::vector<std::string_view> result;
        for (int i = 0; i < str.size(); ++i)
        {
            if (auto foundDelim = containsAt(str, i, dividers))
            {
                result.emplace_back(str.substr(0, i));
                str.remove_prefix(foundDelim->size() + result.back().size());
                i = -1;
            }
        }
        result.emplace_back(str);
        return result;
    }

    INLINE std::optional<std::pair<std::string_view, std::string_view>> tryBreak(
        std::string_view str, const std::vector<std::string_view> &dividers)
    {
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (auto foundDelim = containsAt(str, i, dividers))
            {
                return std::make_pair(str.substr(0, i), str.substr(i + foundDelim->size()));
            }
        }
        return std::nullopt;
    }

    INLINE std::optional<std::pair<std::string_view, std::string_view>> tryBreakFromEnd(
        std::string_view str, const std::vector<std::string_view> &dividers)
    {
        for (int i = static_cast<int>(str.size()) - 1; i >= 0; --i)
        {
            if (auto foundDelim = containsAtFromEnd(str, i, dividers))
            {
                return std::make_pair(str.substr(0, i + 1 - foundDelim->size()), str.substr(i + 1));
            }
        }
        return std::nullopt;
    }

    INLINE std::string joinViews(const std::vector<std::string_view> &strings, const std::string &divider)
    {
        std::string res;
        if (strings.empty())
        {
            return res;
        }
        for (size_t i = 0; i < strings.size() - 1; ++i)
        {
            res += std::string{strings[i]} + divider;
        }
        res += strings.back();
        return res;
    }
} // namespace sb::cf::details::utils