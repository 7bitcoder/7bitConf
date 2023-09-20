#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IDeserializer.hpp"
#include "SevenBit/Conf/ISettingParser.hpp"
#include "SevenBit/Conf/ISettingSplitter.hpp"
#include "SevenBit/Conf/IValueDeserializers.hpp"
#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf
{

    class SettingParserBuilder
    {
      private:
        ISettingSplitter::Ptr _splitter;
        IValueDeserializers::Ptr _valueDeserializers;
        std::vector<std::pair<std::string, IDeserializer::Ptr>> _deserializersMap;
        std::optional<SettingParserConfig> _config;

      public:
        SettingParserBuilder &useSplitter(ISettingSplitter::Ptr splitter);

        SettingParserBuilder &useValueDeserializers(IValueDeserializers::Ptr valueDeserializers);

        SettingParserBuilder &useDeserializerFor(std::string type, IDeserializer::Ptr deserializer);

        SettingParserBuilder &useConfig(SettingParserConfig config);

        ISettingParser::Ptr build();

      private:
        ISettingSplitter::Ptr getSplitter();
        IValueDeserializers::Ptr getValueDeserializers();
        SettingParserConfig &getConfig();
    };

} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParserBuilder.hpp"
#endif