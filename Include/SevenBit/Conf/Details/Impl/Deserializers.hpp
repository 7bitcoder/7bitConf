#pragma once

#include <string>
#include <tao/json/from_string.hpp>

#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/StringUtils.hpp"

namespace sb::cf::details
{
    INLINE JsonValue StringDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return std::string{value ? *value : ""};
    }

    INLINE JsonValue BoolDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value && StringUtils::convertTo<bool>(*value);
    }

    INLINE JsonValue IntDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value ? StringUtils::convertTo<std::int64_t>(*value) : 0;
    }

    INLINE JsonValue UIntDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value ? StringUtils::convertTo<std::uint64_t>(*value) : 0;
    }

    INLINE JsonValue DoubleDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value ? StringUtils::convertTo<double>(*value) : 0.0;
    }

    INLINE JsonValue NullDeserializer::deserialize(std::optional<std::string_view>) const { return json::null; }

    INLINE JsonValue JsonDeserializer::deserialize(std::optional<std::string_view> value) const
    {
        return value ? json::basic_from_string<JsonTraits>(*value) : JsonValue{};
    }
} // namespace sb::cf::details
