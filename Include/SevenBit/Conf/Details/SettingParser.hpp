#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/IJsonDeserializer.hpp"
#include "SevenBit/Conf/Details/JsonDeserializers.hpp"
#include "SevenBit/Conf/Details/SettingParserResult.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"

namespace sb::cf::details
{
    class EXPORT SettingParser
    {
      private:
        SettingParserConfig _config;

      public:
        SettingParser(SettingParserConfig cfg = {});

        SettingParserResult parse(std::string_view setting) const;

        SettingParserResult parseKey(std::string_view key) const;

      private:
        std::string_view SettingParser::checkAndPrepareKey(std::string_view key) const;

        std::size_t tryFindDividersAt(std::string_view key, size_t index, std::string_view divider,
                                      std::string_view alternativeDivider) const;
        std::size_t tryFindDividerAt(std::string_view key, size_t index, std::string_view divider) const;

        std::string_view extractElement(std::string_view &key, size_t &index, size_t dividerSize) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif