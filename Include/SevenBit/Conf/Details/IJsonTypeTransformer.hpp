#pragma once

#include <optional>
#include <string_view>
#include <tao/json/from_string.hpp>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"

namespace sb::cf::details
{
    struct IJsonTypeTransformer
    {
        using Ptr = std::unique_ptr<IJsonTypeTransformer>;

        virtual JsonValue transform(std::optional<std::string_view> value) const = 0;

        virtual ~IJsonTypeTransformer() = default;
    };

    struct StringTransformer : public IJsonTypeTransformer
    {
        JsonValue transform(std::optional<std::string_view> value) const { return std::string{value ? *value : ""}; }
    };

    struct JsonTransformer : public IJsonTypeTransformer
    {
        JsonValue transform(std::optional<std::string_view> value) const
        {
            return value ? json::basic_from_string<JsonTraits>(*value) : JsonValue();
        }
    };

    struct IntTransformer : public IJsonTypeTransformer
    {
        JsonValue transform(std::optional<std::string_view> value) const
        {
            return value ? details::utils::stringTo<std::int64_t>(*value) : 0;
        }
    };

    struct UIntTransformer : public IJsonTypeTransformer
    {
        JsonValue transform(std::optional<std::string_view> value) const
        {
            return value ? details::utils::stringTo<std::uint64_t>(*value) : 0;
        }
    };

    struct BoolTransformer : public IJsonTypeTransformer
    {
        virtual JsonValue transform(std::optional<std::string_view> value) const
        {
            return value && details::utils::stringTo<bool>(*value);
        }
    };

    struct DoubleTransformer : public IJsonTypeTransformer
    {
        virtual JsonValue transform(std::optional<std::string_view> value) const
        {
            return value ? details::utils::stringTo<double>(*value) : 0.0;
        }
    };

    struct NullTransformer : public IJsonTypeTransformer
    {
        virtual JsonValue transform(std::optional<std::string_view> value) const { return json::null; }
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif