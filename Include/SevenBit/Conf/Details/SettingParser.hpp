#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "JsonExt.hpp"
#include "SevenBit/Conf/Details/JsonDeserializers.hpp"
#include "SevenBit/Conf/Details/SettingKeySplitter.hpp"
#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/SettingTypeSplitter.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf::details
{

    class EXPORT SettingParser
    {
      private:
        using KeysTypeAndValue =
            std::tuple<std::vector<std::string_view>, std::optional<std::string_view>, std::optional<std::string_view>>;
        using KeysAndValue = std::pair<std::vector<std::string_view>, JsonValue>;

        const SettingParserConfig _config;
        SettingSplitter _settingSplitter;
        SettingTypeSplitter _settingTypeSplitter;
        SettingKeySplitter _settingKeySplitter;
        JsonDeserializers _deserializers;

      public:
        SettingParser(SettingParserConfig config = {});

        JsonObject parse(std::string_view setting) const;

        void parseInto(std::string_view setting, JsonObject &result) const;

        template <class It> JsonObject parseAll(It begin, It end) const
        {
            JsonObject result;
            parseAllInto(begin, end, result);
            return result;
        }

        template <class It> void parseAllInto(It begin, It end, JsonObject &result) const
        {
            for (auto it = begin; it != end; ++it)
            {
                parseInto(*it, result);
            }
        }

      private:
        KeysAndValue getKeysAndValue(std::string_view setting) const;

        KeysTypeAndValue getKeysTypeAndValue(std::string_view setting) const;

        void checkKeys(const std::vector<std::string_view> &keys) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif