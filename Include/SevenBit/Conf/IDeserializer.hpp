#pragma once

#include <memory>
#include <optional>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    struct IDeserializer
    {
        using Ptr = std::unique_ptr<IDeserializer>;

        [[nodiscard]] virtual JsonValue deserialize(std::optional<std::string_view> value) const = 0;

        virtual ~IDeserializer() = default;
    };
} // namespace sb::cf
