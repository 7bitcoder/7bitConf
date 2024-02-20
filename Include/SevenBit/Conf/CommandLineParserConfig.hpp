#pragma once

#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    struct CommandLineParserConfig
    {
        std::vector<std::string_view> optionPrefixes = {"--", "/"};
        std::vector<std::string_view> optionSplitters = {"=", " "};
        std::vector<std::string_view> optionKeySplitters = {":"};
        std::vector<std::string_view> optionTypeMarkers = {"!"};
        std::string_view defaultType = "string";
        bool throwOnUnknownType = true;
        bool allowEmptyKeys = false;
    };
} // namespace sb::cf
