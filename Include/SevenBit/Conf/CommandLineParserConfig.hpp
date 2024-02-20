#pragma once

#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    struct CommandLineParserConfig
    {
        std::vector<std::string_view> prefixes = {"--", "/"};
        std::vector<std::string_view> splitters = {"=", " "};
        std::vector<std::string_view> keySplitters = {":"};
        std::vector<std::string_view> typeMarkers = {"!"};
        std::string_view defaultType = "string";
        bool throwOnUnknownType = true;
        bool allowEmptyKeys = false;
    };
} // namespace sb::cf
