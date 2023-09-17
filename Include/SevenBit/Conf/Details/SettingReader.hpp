#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "JsonExt.hpp"
#include "SevenBit/Conf/Details/IJsonDeserializer.hpp"
#include "SevenBit/Conf/Details/JsonDeserializers.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"

namespace sb::cf::details
{
    class EXPORT SettingReader
    {
      private:
        SettingParser _parser;
        JsonDeserializers _deserializers;

      public:
        SettingReader(SettingParserConfig config = {}) : _parser(config) {}

        template <class It> JsonObject read(It begin, It end) const
        {
            JsonObject result;
            readInto(begin, end, result);
            return result;
        }

        template <class It> void readInto(It begin, It end, JsonObject &result) const
        {
            for (auto it = begin; it != end; ++it)
            {
                readInto(*it, result);
            }
        }

        void readInto(std::string_view setting, JsonObject &result) const
        {
            try
            {
                auto parserResult = _parser.parse(setting);
                auto &deserializer = _deserializers.getDeserializer(parserResult.getType());
                JsonExt::updateWith(result, parserResult.getKey(), deserializer.deserialize(parserResult.getValue()));
            }
            catch (const std::exception &e)
            {
                throw SettingParserException{"Error for setting: '" + std::string{setting} + " ' error: " + e.what()};
            }
        }
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#endif