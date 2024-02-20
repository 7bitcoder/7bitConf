#pragma once

#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    struct SettingParserConfig
    {
        std::vector<std::string_view> settingPrefixes;
        std::vector<std::string_view> settingSplitters;
        std::vector<std::string_view> keySplitters;
        std::vector<std::string_view> typeMarkers;
        std::string_view defaultType = "string";
        bool throwOnUnknownType = true;
        bool allowEmptyKeys = false;
    };
} // namespace sb::cf
