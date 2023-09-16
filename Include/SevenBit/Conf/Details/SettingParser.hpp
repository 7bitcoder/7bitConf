#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/IJsonTransformer.hpp"
#include "SevenBit/Conf/Details/Setting.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"

namespace sb::cf::details
{
    using JsonTransformersLookup = std::vector<std::pair<std::string_view, IJsonTransformer::Ptr>>;

    class EXPORT SettingParser
    {
      private:
        SettingParserConfig _config;
        JsonTransformersLookup _transformers;

      public:
        SettingParser(JsonTransformersLookup transformers, SettingParserConfig cfg);

        Setting parse(std::string_view setting) const;

        Setting parse(std::string_view key, std::optional<std::string_view> value) const;

      private:
        IJsonTransformer &getTransformer(std::string_view &key) const;

        bool tryExtractType(std::string_view &key, std::string_view type) const;

        bool tryExtractTypeMarker(std::string_view &key, std::string_view typeMarker) const;

        std::string sanitizeKey(std::string_view key) const;

        std::vector<std::string_view> parseKey(std::string_view key) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif