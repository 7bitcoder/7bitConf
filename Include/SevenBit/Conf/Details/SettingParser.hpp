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
    class EXPORT SettingParser
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
        explicit SettingParser(SettingParserConfig config = {});

        JsonObject parseSetting(std::string_view setting) const;

        JsonObject parseSetting(std::string_view key, std::optional<std::string_view> value) const;

      private:
        std::vector<std::string_view> parseKey(std::string_view key) const;

        JsonValue parseValue(SettingType type, std::optional<std::string_view> value) const;

        SettingType extractType(std::string_view &value) const;

        bool tryExtractType(std::string_view &value, std::string_view typeStr) const;

        bool tryExtractTypeMarker(std::string_view &value, std::string_view typeMarker) const;

        JsonObject parseSetting(const std::vector<std::string_view> &key, JsonValue value) const;

        std::string sanitizeKey(std::string_view key) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif