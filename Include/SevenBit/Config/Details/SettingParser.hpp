#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/Details/Utils.hpp"
#include "SevenBit/Config/Json.hpp"
#include "SevenBit/Config/OptionsParserConfig.hpp"

namespace sb::cf::details
{
    EXPORT class SettingParser
    {
      private:
        enum SettingType
        {
            Int,
            UInt,
            Null,
            Bool,
            String,
            Double,
            Json
        };

        SettingParserConfig _config;

      public:
        SettingParser(SettingParserConfig config = {});

        JsonObject parseSetting(std::string_view setting) const;

        JsonObject parseSetting(std::string_view key, std::optional<std::string_view> value) const;

      private:
        std::vector<std::string_view> parseKey(std::string_view key) const;

        JsonValue parseValue(SettingType type, std::optional<std::string_view> value) const;

        SettingType extractType(std::string_view &value) const;

        bool tryExtractType(std::string_view &value, std::string_view typeStr) const;

        JsonObject parseSetting(const std::vector<std::string_view> &key, JsonValue value) const;

        std::string sanitizeKey(std::string_view key) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/SettingParser.hpp"
#endif