#pragma once

#include "SevenBit/Conf/Details/IntDeserializer.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"

namespace sb::cf::details
{
    INLINE JsonValue IntDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value ? details::utils::stringTo<std::int64_t>(*value) : 0;
    }
} // namespace sb::cf::details
