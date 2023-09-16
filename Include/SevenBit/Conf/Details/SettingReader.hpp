#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/IJsonTypeTransformer.hpp"
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
        explicit SettingReader(SettingParserConfig config = {}) : _parser(makeTransformarMap(), std::move(config)) {}

        JsonObject read(std::string_view setting) const;

        template <class It> JsonObject read(It begin, It end) const
        {
            JsonObject result;
            for (auto it = begin; it != end; ++it)
            {
                details::JsonObjectExt::deepMerge(result, _parser.parseSetting(*it));
            }
            return result;
        }

        JsonTransformersMap makeTransformarMap() const
        {
            JsonTransformersMap map;
            map.emplace_back("string", std::make_unique<StringTransformer>());
            map.emplace_back("bool", std::make_unique<BoolTransformer>());
            map.emplace_back("int", std::make_unique<IntTransformer>());
            map.emplace_back("uint", std::make_unique<UIntTransformer>());
            return map;
        }
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif