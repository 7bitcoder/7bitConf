#pragma once

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{

    struct ISettingSplitter
    {
        using Ptr = std::unique_ptr<ISettingSplitter>;

        struct Result
        {
            std::vector<std::string_view> keys;
            std::optional<std::string_view> type;
            std::optional<std::string_view> value;
        };

        virtual Result split(std::string_view setting) const = 0;

        virtual ~ISettingSplitter() = default;
    };

    inline bool operator==(const ISettingSplitter::Result &lhs, const ISettingSplitter::Result &rhs)
    {
        return lhs.keys == rhs.keys && lhs.type == rhs.type && lhs.value == rhs.value;
    }
} // namespace sb::cf
