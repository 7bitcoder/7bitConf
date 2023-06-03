#pragma once

#include <string_view>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    struct SettingParserConfig
    {
        std::string_view settingPrefix = "--";
        std::string_view settingSplitter = "=";
        std::string_view keySplitter = ":";
        std::string_view typeMarker = "!";
        std::string_view alternativeKeySplitter = "__";
    };
} // namespace sb::cf