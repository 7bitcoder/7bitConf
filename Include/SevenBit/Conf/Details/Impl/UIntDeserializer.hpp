#pragma once

#include "SevenBit/Conf/Details/UIntDeserializer.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"

namespace sb::cf::details
{
    INLINE JsonValue UIntDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value ? details::utils::stringTo<std::uint64_t>(*value) : 0;
    }
} // namespace sb::cf::details
