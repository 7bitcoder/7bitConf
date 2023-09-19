#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf::details
{
    class EXPORT SettingSplitter
    {
      private:
        const std::vector<std::string_view> _settingPrefixes;
        const std::vector<std::string_view> _settingSplitters;
        const std::vector<std::string_view> _typeMarkers;
        const std::vector<std::string_view> _keySplitters;

      public:
        struct Result
        {
            std::vector<std::string_view> keys;
            std::optional<std::string_view> type;
            std::optional<std::string_view> value;
        };

        SettingSplitter(std::vector<std::string_view> settingPrefixes, std::vector<std::string_view> settingSplitters,
                        std::vector<std::string_view> typeMarkers, std::vector<std::string_view> keySplitters);

        Result split(std::string_view setting) const;

      private:
        std::string_view tryRemovePrefix(std::string_view setting) const;
        std::vector<std::string_view> splitKey(std::string_view key) const;
        std::pair<std::string_view, std::optional<std::string_view>> splitType(std::string_view key) const;
        std::pair<std::string_view, std::optional<std::string_view>> splitSetting(std::string_view setting) const;
    };

    bool operator==(const SettingSplitter::Result &lhs, const SettingSplitter::Result &rhs);
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingSplitter.hpp"
#endif