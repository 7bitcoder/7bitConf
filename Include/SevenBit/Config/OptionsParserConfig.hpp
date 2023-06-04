#pragma once

#include <string_view>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/Json.hpp"

namespace sb::cf
{
    struct SettingParserConfig
    {
        std::string_view settingPrefix = "--";
        std::string_view settingSplitter = "=";
        std::string_view keySplitter = ":";
        std::string_view typeMarker = "!";
        std::string_view alternativeKeySplitter = "__";
        std::string_view alternativeTypeMarker = "___";
    };
} // namespace sb::cf