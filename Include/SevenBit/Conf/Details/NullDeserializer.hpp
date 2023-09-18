#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/IJsonDeserializer.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    struct NullDeserializer final : public IJsonDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/NullDeserializer.hpp"
#endif
