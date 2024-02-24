#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/EnvironmentVarsParserConfig.hpp"
#include "SevenBit/Conf/ISettingSplitter.hpp"
#include "SevenBit/Conf/ISettingsParser.hpp"
#include "SevenBit/Conf/IValueDeserializersMap.hpp"

namespace sb::cf
{

    class EXPORT EnvironmentVarsParserBuilder
    {
      private:
        ISettingSplitter::Ptr _splitter;
        IValueDeserializersMap::Ptr _valueDeserializersMap;
        std::vector<std::pair<std::string_view, IDeserializer::Ptr>> _valueDeserializers;
        std::optional<EnvironmentVarsParserConfig> _config;

      public:
        EnvironmentVarsParserBuilder &useSplitter(ISettingSplitter::Ptr splitter);

        EnvironmentVarsParserBuilder &useValueDeserializersMap(IValueDeserializersMap::Ptr valueDeserializersMap);

        EnvironmentVarsParserBuilder &useValueDeserializer(std::string_view type, IDeserializer::Ptr valueDeserializer);

        EnvironmentVarsParserBuilder &useConfig(EnvironmentVarsParserConfig config);

        EnvironmentVarsParserBuilder &useDefaultValueDeserializers();

        ISettingsParser::Ptr build();

      private:
        ISettingSplitter::Ptr getSplitter();

        IValueDeserializersMap::Ptr getValueDeserializersMap();

        std::vector<std::pair<std::string_view, IDeserializer::Ptr>> getValueDeserializers();

        EnvironmentVarsParserConfig &getConfig();
    };

} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Impl/EnvironmentVarsParserBuilder.hpp"
#endif
