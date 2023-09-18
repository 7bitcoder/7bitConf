#pragma once

#include "SevenBit/Conf/Details/JsonDeserializer.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"

namespace sb::cf::details
{
    INLINE JsonValue JsonDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value ? json::basic_from_string<JsonTraits>(*value) : JsonValue();
    }
} // namespace sb::cf::details
