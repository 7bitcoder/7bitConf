#pragma once

#include <algorithm>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf::details
{
    struct ContainerUtils
    {
        template <class TIt, class TPred> static TIt removeIf(TIt first, TIt last, TPred &&p)
        {
            first = std::find_if(first, last, p);
            if (first != last)
            {
                for (auto i = first; ++i != last;)
                {
                    if (!p(*i))
                    {
                        *first++ = std::move(*i);
                    }
                }
            }
            return first;
        }

        template <class T, class TPred> static size_t eraseIf(std::vector<T> &vec, TPred &&p)
        {
            const auto it = removeIf(vec.begin(), vec.end(), std::forward<TPred>(p));
            const size_t removedCnt = vec.end() - it;
            vec.erase(it, vec.end());
            return removedCnt;
        }
    };
} // namespace sb::cf::details
