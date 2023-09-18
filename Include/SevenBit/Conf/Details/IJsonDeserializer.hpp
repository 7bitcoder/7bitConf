#pragma once

#include <memory>
#include <optional>
#include <string_view>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    struct IJsonDeserializer
    {
        using Ptr = std::unique_ptr<IJsonDeserializer>;

        virtual JsonValue deserialize(std::optional<std::string_view> value) const = 0;

        virtual ~IJsonDeserializer() = default;
    };
} // namespace sb::cf::details
