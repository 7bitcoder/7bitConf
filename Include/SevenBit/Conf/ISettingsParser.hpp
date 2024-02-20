#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    struct ISettingsParser
    {
        using Ptr = std::unique_ptr<ISettingsParser>;

        [[nodiscard]] virtual JsonObject parse(const std::vector<std::string_view> &settings) const = 0;

        virtual ~ISettingsParser() = default;
    };
} // namespace sb::cf
