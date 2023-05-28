#pragma once

#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"

namespace sb::cf
{
    EXPORT class OptionsParser
    {
        OptionsParserConfig _config;

      public:
        OptionsParser(OptionsParserConfig config = {});

        JsonObject parseOption(std::string_view option) const;

        JsonObject parseOption(std::string_view key, std::string_view value) const;

      private:
        JsonObject parseOption(std::string_view key, JsonValue value) const;

        JsonObject parseOption(const std::vector<std::string_view> &key, JsonValue value) const;

        std::string sanitizeKey(std::string_view key) const;

        JsonValue parseValue(std::string_view &value) const;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/OptionsParser.hpp"
#endif