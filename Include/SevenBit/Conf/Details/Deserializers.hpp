#pragma once

#include <memory>
#include <optional>
#include <string_view>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IDeserializer.hpp"

namespace sb::cf::details
{
    struct EXPORT StringDeserializer final : public IDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const;
    };

    struct EXPORT BoolDeserializer final : public IDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const;
    };

    struct EXPORT IntDeserializer final : public IDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const;
    };

    struct EXPORT UIntDeserializer final : public IDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const;
    };

    struct EXPORT DoubleDeserializer final : public IDeserializer
    {
        virtual JsonValue deserialize(std::optional<std::string_view> value) const;
    };

    struct EXPORT JsonDeserializer final : public IDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const;
    };

    struct EXPORT NullDeserializer final : public IDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/Deserializers.hpp"
#endif