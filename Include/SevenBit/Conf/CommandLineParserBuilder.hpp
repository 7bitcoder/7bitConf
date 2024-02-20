#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/CommandLineParserConfig.hpp"
#include "SevenBit/Conf/ISettingSplitter.hpp"
#include "SevenBit/Conf/ISettingsParser.hpp"
#include "SevenBit/Conf/IValueDeserializersMap.hpp"

namespace sb::cf
{

    class EXPORT CommandLineParserBuilder
    {
      private:
        ISettingSplitter::Ptr _splitter;
        IValueDeserializersMap::Ptr _valueDeserializersMap;
        std::vector<std::pair<std::string_view, IDeserializer::Ptr>> _valueDeserializers;
        std::optional<CommandLineParserConfig> _config;

      public:
        CommandLineParserBuilder &useSplitter(ISettingSplitter::Ptr splitter);

        CommandLineParserBuilder &useValueDeserializersMap(IValueDeserializersMap::Ptr valueDeserializersMap);

        CommandLineParserBuilder &useValueDeserializer(std::string_view type, IDeserializer::Ptr valueDeserializer);

        CommandLineParserBuilder &useConfig(CommandLineParserConfig config);

        CommandLineParserBuilder &useDefaultValueDeserializers();

        ISettingsParser::Ptr build();

      private:
        bool tryRemoveWhiteSpaceSplitters();

        ISettingSplitter::Ptr getSplitter();

        IValueDeserializersMap::Ptr getValueDeserializersMap();

        CommandLineParserConfig &getConfig();
    };

} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Impl/CommandLineParserBuilder.hpp"
#endif
