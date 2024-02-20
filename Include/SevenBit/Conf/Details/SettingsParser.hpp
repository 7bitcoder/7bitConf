#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ISettingSplitter.hpp"
#include "SevenBit/Conf/ISettingsParser.hpp"
#include "SevenBit/Conf/IValueDeserializersMap.hpp"

namespace sb::cf::details
{
    class EXPORT SettingsParser : public ISettingsParser
    {
        struct ArgumentParseResult
        {
            std::vector<std::string_view> keys;
            JsonValue value;
        };

        const ISettingSplitter::Ptr _settingSplitter;
        const IValueDeserializersMap::Ptr _valueDeserializersMap;
        const std::vector<std::string_view> _settingPrefixes;
        bool _combinedSetting;

      public:
        using Ptr = std::unique_ptr<SettingsParser>;

        SettingsParser(ISettingSplitter::Ptr settingSplitter, IValueDeserializersMap::Ptr valueDeserializersMap,
                       std::vector<std::string_view> settingPrefixes, bool combinedSetting);

        [[nodiscard]] JsonObject parse(const std::vector<std::string_view> &settings) const override;

      private:
        [[nodiscard]] ArgumentParseResult parseArgument(const std::vector<std::string_view> &settings,
                                                        size_t &index) const;

        bool tryRemovePrefix(std::string_view &setting) const;

        [[nodiscard]] std::optional<std::string_view> tryGetPrefix(std::string_view setting) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingsParser.hpp"
#endif
