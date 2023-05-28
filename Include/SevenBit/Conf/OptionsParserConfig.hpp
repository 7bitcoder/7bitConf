#pragma once

#include <string_view>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    struct OptionsParserConfig
    {
        std::string_view optPrefix = "--";
        std::string_view optSplitter = "=";
        std::string_view keySplitter = ":";
        std::string_view alternativeKeySplitter = "__";
        std::string_view valueListSplitter = ";";
        JsonValue defaultValue = json::uninitialized;
    };
} // namespace sb::cf