#pragma once

#include <memory>
#include <optional>
#include <string_view>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    struct IDeserializer
    {
        using Ptr = std::unique_ptr<IDeserializer>;

        virtual JsonValue deserialize(std::optional<std::string_view> value) const = 0;

        virtual ~IDeserializer() = default;
    };
} // namespace sb::cf::details
