#pragma once

#include <algorithm>

#include "SevenBit/Conf/CommandLineParserBuilder.hpp"
#include "SevenBit/Conf/Details/CommandLineParser.hpp"
#include "SevenBit/Conf/Details/ContainerUtils.hpp"
#include "SevenBit/Conf/Details/DefaultDeserializers.hpp"
#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/StringUtils.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"

namespace sb::cf
{
    INLINE CommandLineParserBuilder &CommandLineParserBuilder::useSplitter(ISettingSplitter::Ptr splitter)
    {
        _splitter = std::move(splitter);
        return *this;
    }

    INLINE CommandLineParserBuilder &CommandLineParserBuilder::useValueDeserializersMap(
        IValueDeserializersMap::Ptr valueDeserializersMap)
    {
        _valueDeserializersMap = std::move(valueDeserializersMap);
        return *this;
    }

    INLINE CommandLineParserBuilder &CommandLineParserBuilder::useValueDeserializer(
        std::string_view type, IDeserializer::Ptr valueDeserializer)
    {
        _valueDeserializers.emplace_back(type, std::move(valueDeserializer));
        return *this;
    }

    INLINE CommandLineParserBuilder &CommandLineParserBuilder::useConfig(CommandLineParserConfig config)
    {
        _config = std::move(config);
        return *this;
    }

    INLINE CommandLineParserBuilder &CommandLineParserBuilder::useDefaultValueDeserializers()
    {
        details::DefaultDeserializers::add(_valueDeserializers);
        return *this;
    }

    INLINE ISettingsParser::Ptr CommandLineParserBuilder::build()
    {
        auto &config = getConfig();
        auto hadWhiteSpaceSplitters = details::ContainerUtils::eraseIf(
            config.optionSplitters, [](const auto splitter) { return details::StringUtils::isWhiteSpace(splitter); });

        return std::make_unique<details::CommandLineParser>(getSplitter(), getValueDeserializersMap(),
                                                            std::move(config.optionPrefixes), hadWhiteSpaceSplitters);
    }

    INLINE ISettingSplitter::Ptr CommandLineParserBuilder::getSplitter()
    {
        if (!_splitter)
        {
            auto &config = getConfig();
            useSplitter(std::make_unique<details::SettingSplitter>(
                std::move(config.optionSplitters), std::move(config.typeMarkers), std::move(config.keySplitters),
                config.allowEmptyKeys));
        }
        return std::move(_splitter);
    }

    INLINE IValueDeserializersMap::Ptr CommandLineParserBuilder::getValueDeserializersMap()
    {
        if (!_valueDeserializersMap)
        {
            auto &config = getConfig();
            useValueDeserializersMap(std::make_unique<details::ValueDeserializersMap>(
                config.defaultType, config.throwOnUnknownType, getValueDeserializers()));
        }
        return std::move(_valueDeserializersMap);
    }

    INLINE std::vector<std::pair<std::string_view, IDeserializer::Ptr>> CommandLineParserBuilder::
        getValueDeserializers()
    {
        if (_valueDeserializers.empty())
        {
            useDefaultValueDeserializers();
        }
        return std::move(_valueDeserializers);
    }

    INLINE CommandLineParserConfig &CommandLineParserBuilder::getConfig()
    {
        if (!_config)
        {
            useConfig({});
        }
        return *_config;
    }
} // namespace sb::cf
