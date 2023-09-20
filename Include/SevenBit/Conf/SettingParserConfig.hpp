#pragma once

#include <string_view>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    struct SettingParserConfig
    {
        std::vector<std::string_view> settingPrefixes = {"--"};
        std::vector<std::string_view> settingSplitters = {"="};
        std::vector<std::string_view> keySplitters = {":", "__"};
        std::vector<std::string_view> typeMarkers = {"!", "___"};
        std::string_view presumedType = "string";
        bool throwOnUnknownType = true;
        bool allowEmptyKeys = false;
    };
} // namespace sb::cf