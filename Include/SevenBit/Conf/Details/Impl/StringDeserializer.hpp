#pragma once

#include <string>

#include "SevenBit/Conf/Details/StringDeserializer.hpp"

namespace sb::cf::details
{
    INLINE JsonValue StringDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return std::string{value ? *value : ""};
    }
} // namespace sb::cf::details
