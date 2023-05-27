#pragma once
#include <stdexcept>
#include <string_view>
#include <tao/json/type.hpp>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/OptionsParserCfg.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    EXPORT class OptionsParser
    {
        OptionsParserCfg _cfg;

      public:
        OptionsParser(OptionsParserCfg cfg = {});

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