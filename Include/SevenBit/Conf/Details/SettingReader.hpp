#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/IJsonTransformer.hpp"
#include "SevenBit/Conf/Details/JsonTransformersLookup.hpp"
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

      public:
        explicit SettingReader(SettingParserConfig config = {})
            : _parser(JsonTransformersLookup::createDefault(), std::move(config))
        {
        }

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
                details::JsonExt::updateWithSetting(result, _parser.parse(*it));
            }
        }
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#endif