#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    struct ISettingParser
    {
        using Ptr = std::unique_ptr<ISettingParser>;

        struct Result
        {
            std::vector<std::string_view> keys;
            JsonValue value;
        };

        [[nodiscard]] virtual Result parse(std::string_view setting) const = 0;

        virtual ~ISettingParser() = default;
    };

    inline bool operator==(const ISettingParser::Result &lhs, const ISettingParser::Result &rhs)
    {
        return lhs.keys == rhs.keys && lhs.value == rhs.value;
    }
} // namespace sb::cf
