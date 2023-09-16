#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/IJsonTransformer.hpp"
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
        explicit SettingReader(SettingParserConfig config = {}) : _parser(makeTransformersLookup(), std::move(config))
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

        JsonTransformersLookup makeTransformersLookup() const
        {
            JsonTransformersLookup lookup;
            lookup.emplace_back("string", std::make_unique<StringTransformer>());
            lookup.emplace_back("bool", std::make_unique<BoolTransformer>());
            lookup.emplace_back("int", std::make_unique<IntTransformer>());
            lookup.emplace_back("double", std::make_unique<DoubleTransformer>());
            lookup.emplace_back("json", std::make_unique<JsonTransformer>());
            lookup.emplace_back("uint", std::make_unique<UIntTransformer>());
            lookup.emplace_back("null", std::make_unique<NullTransformer>());
            return lookup;
        }
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#endif