#pragma once

#include "SevenBit/Conf/Details/DefaultDeserializers.hpp"
#include "SevenBit/Conf/Details/EnvironmentVarsParser.hpp"
#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/EnvironmentVarsParserBuilder.hpp"

namespace sb::cf
{
    INLINE EnvironmentVarsParserBuilder &EnvironmentVarsParserBuilder::useSplitter(ISettingSplitter::Ptr splitter)
    {
        _splitter = std::move(splitter);
        return *this;
    }

    INLINE EnvironmentVarsParserBuilder &EnvironmentVarsParserBuilder::useValueDeserializersMap(
        IValueDeserializersMap::Ptr valueDeserializersMap)
    {
        _valueDeserializersMap = std::move(valueDeserializersMap);
        return *this;
    }

    INLINE EnvironmentVarsParserBuilder &EnvironmentVarsParserBuilder::useValueDeserializer(
        std::string_view type, IDeserializer::Ptr valueDeserializer)
    {
        _valueDeserializers.emplace_back(type, std::move(valueDeserializer));
        return *this;
    }

    INLINE EnvironmentVarsParserBuilder &EnvironmentVarsParserBuilder::useConfig(EnvironmentVarsParserConfig config)
    {
        _config = std::move(config);
        return *this;
    }

    INLINE EnvironmentVarsParserBuilder &EnvironmentVarsParserBuilder::useDefaultValueDeserializers()
    {
        DefaultDeserializers::addDefault(_valueDeserializers);
        return *this;
    }

    INLINE ISettingsParser::Ptr EnvironmentVarsParserBuilder::build()
    {
        return std::make_unique<details::EnvironmentVarsParser>(getSplitter(), getValueDeserializersMap());
    }

    INLINE ISettingSplitter::Ptr EnvironmentVarsParserBuilder::getSplitter()
    {
        if (!_splitter)
        {
            auto &config = getConfig();
            useSplitter(
                std::make_unique<details::SettingSplitter>(std::move(config.splitters), std::move(config.typeMarkers),
                                                           std::move(config.keySplitters), config.allowEmptyKeys));
        }
        return std::move(_splitter);
    }

    INLINE IValueDeserializersMap::Ptr EnvironmentVarsParserBuilder::getValueDeserializersMap()
    {
        if (!_valueDeserializersMap)
        {
            if (_valueDeserializers.empty())
            {
                useDefaultValueDeserializers();
            }
            auto &config = getConfig();
            useValueDeserializersMap(std::make_unique<details::ValueDeserializersMap>(
                config.defaultType, config.throwOnUnknownType, std::move(_valueDeserializers)));
        }
        return std::move(_valueDeserializersMap);
    }

    INLINE EnvironmentVarsParserConfig &EnvironmentVarsParserBuilder::getConfig()
    {
        if (!_config)
        {
            useConfig({});
        }
        return *_config;
    }
} // namespace sb::cf
