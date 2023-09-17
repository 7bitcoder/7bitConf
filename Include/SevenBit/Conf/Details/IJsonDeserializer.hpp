#pragma once

#include <optional>
#include <string_view>
#include <tao/json/from_string.hpp>
#include <utility>
#include <vector>

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

    struct StringDeserializer : public IJsonDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const { return std::string{value ? *value : ""}; }
    };

    struct JsonDeserializer : public IJsonDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const
        {
            return value ? json::basic_from_string<JsonTraits>(*value) : JsonValue();
        }
    };

    struct IntDeserializer : public IJsonDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const
        {
            return value ? details::utils::stringTo<std::int64_t>(*value) : 0;
        }
    };

    struct UIntDeserializer : public IJsonDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const
        {
            return value ? details::utils::stringTo<std::uint64_t>(*value) : 0;
        }
    };

    struct BoolDeserializer : public IJsonDeserializer
    {
        virtual JsonValue deserialize(std::optional<std::string_view> value) const
        {
            return value && details::utils::stringTo<bool>(*value);
        }
    };

    struct DoubleDeserializer : public IJsonDeserializer
    {
        virtual JsonValue deserialize(std::optional<std::string_view> value) const
        {
            return value ? details::utils::stringTo<double>(*value) : 0.0;
        }
    };

    struct NullDeserializer : public IJsonDeserializer
    {
        virtual JsonValue deserialize(std::optional<std::string_view> value) const { return json::null; }
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#endif