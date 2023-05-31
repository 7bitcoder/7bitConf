#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"

namespace sb::cf::details
{
    EXPORT class SettingParser
    {
      private:
        enum SettingType
        {
            Int,
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

        JsonObject parseSetting(std::string_view key, JsonValue value) const;

      private:
        JsonObject parseSetting(const std::vector<std::string_view> &key, JsonValue value) const;

        std::pair<std::vector<std::string_view>, SettingType> parseKey(std::string_view key) const;

        JsonValue parseValue(SettingType type, std::string_view value) const;

        std::string sanitizeKey(std::string_view key) const;

        SettingType extractType(std::string_view &value) const;

        JsonValue parseElementValue(SettingType type, std::string_view value) const;

        JsonValue getDefault(SettingType type) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif