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
    class EXPORT CommandLineParser : public ISettingsParser
    {
        struct ArgumentParseResult
        {
            std::vector<std::string_view> keys;
            JsonValue value;
        };

        const ISettingSplitter::Ptr _optionSplitter;
        const IValueDeserializersMap::Ptr _valueDeserializersMap;
        const std::vector<std::string_view> _optionPrefixes;
        const bool _considerSeparated;

      public:
        using Ptr = std::unique_ptr<CommandLineParser>;

        CommandLineParser(ISettingSplitter::Ptr optionSplitter, IValueDeserializersMap::Ptr valueDeserializersMap,
                          std::vector<std::string_view> optionPrefixes, bool considerSeparated = true);

        [[nodiscard]] JsonObject parse(const std::vector<std::string_view> &arguments) const override;

      private:
        [[nodiscard]] ArgumentParseResult parseArgument(const std::vector<std::string_view> &arguments,
                                                        size_t &index) const;

        bool tryRemoveOptionPrefix(std::string_view &argument) const;

        [[nodiscard]] std::optional<std::string_view> tryGetOptionPrefix(std::string_view argument) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/CommandLineParser.hpp"
#endif
