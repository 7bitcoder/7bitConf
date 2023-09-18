#pragma once

#include "SevenBit/Conf/Details/NullDeserializer.hpp"

namespace sb::cf::details
{
    INLINE JsonValue NullDeserializer::deserialize(std::optional<std::string_view> value) const { return json::null; }
} // namespace sb::cf::details
