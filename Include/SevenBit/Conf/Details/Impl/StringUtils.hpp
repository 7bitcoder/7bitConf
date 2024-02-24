#pragma once

#include <algorithm>
#include <cctype>

#include "SevenBit/Conf/Details/StringUtils.hpp"

namespace sb::cf::details
{
    INLINE bool StringUtils::isNumber(std::string_view str)
    {
        return !str.empty() && std::all_of(str.begin(), str.end(), [](unsigned char ch) { return std::isdigit(ch); });
    }

    INLINE bool StringUtils::ignoreCaseLess(std::string_view str, std::string_view search)
    {
        return std::lexicographical_compare(
            str.begin(), str.end(), search.begin(), search.end(),
            [](unsigned char cha, unsigned char chb) { return std::tolower(cha) < std::tolower(chb); });
    }

    INLINE bool StringUtils::ignoreCaseEqual(std::string_view str, std::string_view search)
    {
        return str.size() == search.size() &&
               std::equal(str.begin(), str.end(), search.begin(), search.end(),
                          [](unsigned char cha, unsigned char chb) { return std::tolower(cha) == std::tolower(chb); });
    }

    INLINE bool StringUtils::containsAt(std::string_view str, size_t index, std::string_view search)
    {
        if (index >= str.size() || search.empty())
        {
            return false;
        }
        return str.compare(index, search.size(), search) == 0;
    }

    INLINE std::optional<std::string_view> StringUtils::containsAt(std::string_view str, size_t index,
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

    INLINE bool StringUtils::containsAtFromEnd(std::string_view str, size_t index, std::string_view search)
    {
        if (index + 1 < search.size())
        {
            return false;
        }
        return containsAt(str, index + 1 - search.size(), search);
    }

    INLINE std::optional<std::string_view> StringUtils::containsAtFromEnd(std::string_view str, size_t index,
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

    INLINE bool StringUtils::startsWith(std::string_view str, std::string_view search)
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

    INLINE size_t StringUtils::startsWithWhiteSpace(std::string_view str)
    {
        const auto it = std::find_if(str.begin(), str.end(), [](const unsigned char ch) { return !std::isspace(ch); });
        return it - str.begin();
    }

    INLINE bool StringUtils::isWhiteSpace(std::string_view str)
    {
        return !str.empty() && startsWithWhiteSpace(str) == str.size();
    }

    INLINE std::vector<std::string_view> StringUtils::split(std::string_view str, std::string_view separator)
    {
        std::vector<std::string_view> result;
        std::string::size_type begin = 0, pos = 0;
        for (; std::string_view::npos != (pos = str.find_first_of(separator, pos)); begin = (pos += separator.size()))
        {
            result.push_back(str.substr(begin, pos - begin));
        }
        result.push_back(str.substr(begin));
        return result;
    }

    INLINE std::vector<std::string_view> StringUtils::split(std::string_view str,
                                                            const std::vector<std::string_view> &separators)
    {
        std::vector<std::string_view> result;
        for (int i = 0; i < str.size(); ++i)
        {
            if (const auto separator = containsAt(str, i, separators))
            {
                result.emplace_back(str.substr(0, i));
                str.remove_prefix(separator->size() + result.back().size());
                i = -1;
            }
        }
        result.emplace_back(str);
        return result;
    }

    INLINE std::optional<std::pair<std::string_view, std::string_view>> StringUtils::tryBreak(
        std::string_view str, const std::vector<std::string_view> &separators)
    {
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (const auto separator = containsAt(str, i, separators))
            {
                return std::make_pair(str.substr(0, i), str.substr(i + separator->size()));
            }
        }
        return std::nullopt;
    }

    INLINE std::optional<std::pair<std::string_view, std::string_view>> StringUtils::tryBreakFromEnd(
        std::string_view str, const std::vector<std::string_view> &separators)
    {
        for (int i = static_cast<int>(str.size()) - 1; i >= 0; --i)
        {
            if (const auto separator = containsAtFromEnd(str, i, separators))
            {
                return std::make_pair(str.substr(0, i + 1 - separator->size()), str.substr(i + 1));
            }
        }
        return std::nullopt;
    }

    INLINE std::string StringUtils::join(const std::vector<std::string_view> &strs, const std::string &separator)
    {
        std::string res;
        if (!strs.empty())
        {
            for (size_t i = 0; i < strs.size() - 1; ++i)
            {
                res += std::string{strs[i]} + separator;
            }
            res += strs.back();
        }
        return res;
    }
} // namespace sb::cf::details
