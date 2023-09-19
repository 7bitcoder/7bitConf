#pragma once

#include <optional>
#include <string_view>
#include <tao/json/from_string.hpp>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/ISettingDeserializer.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    struct BoolDeserializer final : public ISettingDeserializer
    {
        JsonValue deserialize(std::optional<std::string_view> value) const final;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/BoolDeserializer.hpp"
#endif