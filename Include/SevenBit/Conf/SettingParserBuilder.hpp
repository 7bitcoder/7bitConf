#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ISettingParser.hpp"
#include "SevenBit/Conf/ISettingSplitter.hpp"
#include "SevenBit/Conf/IValueDeserializersMap.hpp"
#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf
{

    class EXPORT SettingParserBuilder
    {
      private:
        ISettingSplitter::Ptr _splitter;
        IValueDeserializersMap::Ptr _valueDeserializersMap;
        std::vector<std::pair<std::string_view, IDeserializer::Ptr>> _deserializersMap;
        std::optional<SettingParserConfig> _config;

      public:
        SettingParserBuilder &useSplitter(ISettingSplitter::Ptr splitter);

        SettingParserBuilder &useValueDeserializersMap(IValueDeserializersMap::Ptr valueDeserializersMap);

        SettingParserBuilder &useValueDeserializer(std::string_view type, IDeserializer::Ptr valueDeserializer);

        SettingParserBuilder &useConfig(SettingParserConfig config);

        SettingParserBuilder &useDefaultValueDeserializers();

        ISettingParser::Ptr build();

      private:
        ISettingSplitter::Ptr getSplitter();

        IValueDeserializersMap::Ptr getValueDeserializersMap();

        SettingParserConfig &getConfig();
    };

} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParserBuilder.hpp"
#endif