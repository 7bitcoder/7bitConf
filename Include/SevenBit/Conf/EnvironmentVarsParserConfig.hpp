#pragma once

#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    struct EnvironmentVarsParserConfig
    {
        std::vector<std::string_view> variableSplitters = {"="};
        std::vector<std::string_view> keySplitters = {":", "__"};
        std::vector<std::string_view> typeMarkers = {"!", "___"};
        std::string_view defaultType = "string";
        bool throwOnUnknownType = true;
        bool allowEmptyKeys = false;
    };
} // namespace sb::cf
