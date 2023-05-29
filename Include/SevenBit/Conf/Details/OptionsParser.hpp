#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"

namespace sb::cf
{
    EXPORT class OptionsParser
    {
      private:
        enum OptionType
        {
            Int,
            Bool,
            String,
            Double,
            Json
        };

        OptionsParserConfig _config;

      public:
        OptionsParser(OptionsParserConfig config = {});

        JsonObject parseOption(std::string_view option) const;

        JsonObject parseOption(std::string_view key, std::optional<std::string_view> value) const;

      private:
        JsonObject parseOption(const std::vector<std::string_view> &key, JsonValue value) const;

        std::pair<std::vector<std::string_view>, OptionType> parseKey(std::string_view key) const;

        JsonValue parseValue(OptionType type, std::string_view value) const;

        std::string sanitizeKey(std::string_view key) const;

        OptionType extractType(std::string_view &value) const;

        JsonValue parseElementValue(OptionType type, std::string_view value) const;

        JsonValue getDefault(OptionType type) const;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/OptionsParser.hpp"
#endif