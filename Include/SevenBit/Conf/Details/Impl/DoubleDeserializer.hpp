#pragma once

#include "SevenBit/Conf/Details/DoubleDeserializer.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"

namespace sb::cf::details
{
    INLINE JsonValue DoubleDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value ? details::utils::stringTo<double>(*value) : 0.0;
    }
} // namespace sb::cf::details
