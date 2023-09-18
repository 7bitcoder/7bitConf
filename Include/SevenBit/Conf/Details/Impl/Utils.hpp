#pragma once
#include <algorithm>
#include <cctype>

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details::utils
{
    template <class TIterator, class TFunc>
    bool edgeEqual(TIterator it, TIterator end, TIterator searchIt, TIterator searchEnd, TFunc cmp)
    {
        for (; it != end && searchIt != searchEnd; ++it, ++searchIt)
        {
            if (!cmp(*it, *searchIt))
            {
                return false;
            }
        }
        return searchIt == searchEnd;
    }

    INLINE bool isNumberString(std::string_view str)
    {
        for (auto ch : str)
        {
            if (!std::isdigit(ch))
            {
                return false;
            }
        }
        return !str.empty();
    }

    INLINE bool ignoreCaseEqual(char cha, char chb) { return std::tolower(cha) == std::tolower(chb); }

    INLINE bool ignoreCaseEqual(std::string_view str, std::string_view search)
    {
        return std::equal(str.begin(), str.end(), search.begin(), search.end(),
                          [](char cha, char chb) { return ignoreCaseEqual(cha, chb); });
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

    INLINE bool containsAt(std::string_view str, size_t index, std::string_view search)
    {
        return str.compare(index, search.size(), search) == 0;
    }

    INLINE std::optional<std::string_view> backwardContainsAt(std::string_view str, size_t index,
                                                              const std::vector<std::string_view> &searches)
    {
        for (auto &search : searches)
        {
            if (backwardContainsAt(str, index, search))
            {
                return search;
            }
        }
        return std::nullopt;
    }

    INLINE bool backwardContainsAt(std::string_view str, size_t index, std::string_view search)
    {
        if (index + 1 < search.size())
        {
            return false;
        }
        return str.compare(index + 1 - search.size(), search.size(), search) == 0;
    }

    INLINE bool ignoreCaseContainsAt(std::string_view str, size_t index, std::string_view search)
    {
        auto part = str.substr(index, search.size());
        return ignoreCaseEqual(part, search);
    }

    INLINE bool startsWith(std::string_view str, std::string_view search)
    {
        return edgeEqual(str.begin(), str.end(), search.begin(), search.end(),
                         [](char cha, char chb) { return cha == chb; });
    }

    INLINE bool ignoreCaseStartsWith(std::string_view str, std::string_view search)
    {
        return edgeEqual(str.begin(), str.end(), search.begin(), search.end(),
                         [](char cha, char chb) { return ignoreCaseEqual(cha, chb); });
    }

    INLINE bool endsWith(std::string_view str, std::string_view search)
    {
        return edgeEqual(str.rbegin(), str.rend(), search.rbegin(), search.rend(),
                         [](char cha, char chb) { return cha == chb; });
    }

    INLINE bool ignoreCaseEndsWith(std::string_view str, std::string_view search)
    {
        return edgeEqual(str.rbegin(), str.rend(), search.rbegin(), search.rend(),
                         [](char cha, char chb) { return ignoreCaseEqual(cha, chb); });
    }

    INLINE std::size_t replaceAll(std::string &inout, std::string_view what, std::string_view with)
    {
        std::size_t count = 0;
        for (std::string::size_type pos = 0; std::string::npos != (pos = inout.find(what.data(), pos, what.size()));
             pos += with.size(), ++count)
        {
            inout.replace(pos, what.size(), with);
        }
        return count;
    }

    INLINE std::vector<std::string_view> split(std::string_view str, const std::string_view &delim, size_t max)
    {
        std::vector<std::string_view> result;

        std::string::size_type begin = 0, pos = 0;
        for (size_t cnt = 1; cnt < max && std::string_view::npos != (pos = str.find_first_of(delim, pos));
             begin = (pos += delim.size()), ++cnt)
        {
            result.push_back(str.substr(begin, pos - begin));
        }
        result.push_back(str.substr(begin));
        return result;
    }

    INLINE std::vector<std::string_view> split(std::string_view str, const std::vector<std::string_view> &delims,
                                               size_t max)
    {
        std::vector<std::string_view> result;
        if (max < 2)
        {
            return {str};
        }
        for (size_t i = 0; result.size() < max - 1 && i < str.size(); ++i)
        {
            if (auto foundDelim = containsAt(str, i, delims))
            {
                result.emplace_back(str.substr(0, i));
                str.remove_prefix(foundDelim->size() + result.back().size());
                i = 0;
            }
        }
        result.emplace_back(str);
        return result;
    }

    INLINE std::vector<std::string_view> backwardsSplit(std::string_view str,
                                                        const std::vector<std::string_view> &delims, size_t max)
    {
        std::vector<std::string_view> result;
        if (max < 2)
        {
            return {str};
        }
        size_t i = str.size();
        do
        {
            --i;
            if (auto foundDelim = backwardContainsAt(str, i, delims))
            {
                result.emplace_back(str.substr(i + 1));
                str.remove_suffix(foundDelim->size() + result.back().size());
                i = str.size();
            }
        } while (i && result.size() < max - 1);
        result.emplace_back(str);
        return {result.rbegin(), result.rend()};
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

    INLINE std::string join(const std::vector<std::string> &strings, const std::string &divider)
    {
        std::string res;
        if (strings.empty())
        {
            return res;
        }
        for (size_t i = 0; i < strings.size() - 1; ++i)
        {
            res += strings[i] + divider;
        }
        res += strings.back();
        return res;
    }
} // namespace sb::cf::details::utils