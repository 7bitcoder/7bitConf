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
    struct IJsonTransformer
    {
        using Ptr = std::unique_ptr<IJsonTransformer>;

        virtual JsonValue transform(std::optional<std::string_view> value) const = 0;

        virtual ~IJsonTransformer() = default;
    };

    struct StringTransformer : public IJsonTransformer
    {
        JsonValue transform(std::optional<std::string_view> value) const { return std::string{value ? *value : ""}; }
    };

    struct JsonTransformer : public IJsonTransformer
    {
        JsonValue transform(std::optional<std::string_view> value) const
        {
            return value ? json::basic_from_string<JsonTraits>(*value) : JsonValue();
        }
    };

    struct IntTransformer : public IJsonTransformer
    {
        JsonValue transform(std::optional<std::string_view> value) const
        {
            return value ? details::utils::stringTo<std::int64_t>(*value) : 0;
        }
    };

    struct UIntTransformer : public IJsonTransformer
    {
        JsonValue transform(std::optional<std::string_view> value) const
        {
            return value ? details::utils::stringTo<std::uint64_t>(*value) : 0;
        }
    };

    struct BoolTransformer : public IJsonTransformer
    {
        virtual JsonValue transform(std::optional<std::string_view> value) const
        {
            return value && details::utils::stringTo<bool>(*value);
        }
    };

    struct DoubleTransformer : public IJsonTransformer
    {
        virtual JsonValue transform(std::optional<std::string_view> value) const
        {
            return value ? details::utils::stringTo<double>(*value) : 0.0;
        }
    };

    struct NullTransformer : public IJsonTransformer
    {
        virtual JsonValue transform(std::optional<std::string_view> value) const { return json::null; }
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#endif