#pragma once

#include "SevenBit/Conf/Details/BoolDeserializer.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"

namespace sb::cf::details
{
    INLINE JsonValue BoolDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value && details::utils::stringTo<bool>(*value);
    }
} // namespace sb::cf::details
