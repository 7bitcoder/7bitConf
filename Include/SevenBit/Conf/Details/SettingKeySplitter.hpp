#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/IJsonDeserializer.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf::details
{
    class EXPORT SettingKeySplitter
    {
      private:
        SettingParserConfig _config;

      public:
        struct Result
        {
            std::vector<std::string_view> keys;
            std::string_view type;
        };

        SettingKeySplitter(SettingParserConfig cfg = {});

        Result split(std::string_view key) const;

      private:
        std::string_view checkAndPrepareKey(std::string_view key) const;

        std::size_t tryFindDividersAt(std::string_view key, size_t index, std::string_view divider,
                                      std::string_view alternativeDivider) const;
        std::size_t tryFindDividerAt(std::string_view key, size_t index, std::string_view divider) const;

        std::string_view extractElement(std::string_view &key, size_t &index, size_t dividerSize) const;
    };

    bool operator==(const SettingKeySplitter::Result &lhs, const SettingKeySplitter::Result &rhs);

} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingKeySplitter.hpp"
#endif