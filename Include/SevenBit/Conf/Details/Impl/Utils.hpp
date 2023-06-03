#pragma once
#include <algorithm>
#include <cctype>

#include "SevenBit/Conf/Details/Utils.hpp"

namespace sb::cf::utils
{

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

    INLINE bool ignoreCaseEquals(std::string_view stra, std::string_view strb)
    {
        return std::equal(stra.begin(), stra.end(), strb.begin(), strb.end(),
                          [](char cha, char chb) { return std::tolower(cha) == std::tolower(chb); });
    }

    INLINE bool startsWith(std::string_view str, std::string_view search)
    {
        if (search.empty())
        {
            return true;
        }
        auto strIt = str.begin();
        auto searchIt = search.begin();
        for (; strIt != str.end() && searchIt != search.end(); ++strIt, ++searchIt)
        {
            if (*strIt != *searchIt)
            {
                return false;
            }
        }
        return searchIt == search.end();
    }

    INLINE bool endsWith(std::string_view str, std::string_view search)
    {
        if (search.empty())
        {
            return true;
        }
        auto strIt = str.rbegin();
        auto searchIt = search.rbegin();
        for (; strIt != str.rend() && searchIt != search.rend(); ++strIt, ++searchIt)
        {
            if (*strIt != *searchIt)
            {
                return false;
            }
        }
        return searchIt == search.rend();
    }

    INLINE std::size_t replaceAll(std::string &inout, std::string_view what, std::string_view with)
    {
        std::size_t count = 0;
        for (std::string::size_type pos = 0; inout.npos != (pos = inout.find(what.data(), pos, what.size()));
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
        for (size_t cnt = 1; cnt < max && str.npos != (pos = str.find_first_of(delim, pos));
             begin = (pos += delim.size()), ++cnt)
        {
            result.push_back(str.substr(begin, pos - begin));
        }
        result.push_back(str.substr(begin));
        return result;
    }

    INLINE std::string join(const std::vector<std::string_view> &strs, const std::string &divider)
    {
        std::string res;
        if (strs.empty())
        {
            return res;
        }
        for (size_t i = 0; i < strs.size() - 1; ++i)
        {
            res += std::string{strs[i]} + divider;
        }
        res += strs.back();
        return res;
    }
} // namespace sb::cf::utils