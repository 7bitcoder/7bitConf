#pragma once

#include "SevenBit/Conf/CommandLineParserBuilder.hpp"
#include "SevenBit/Conf/Details/CommandLineParser.hpp"
#include "SevenBit/Conf/Details/DefaultDeserializers.hpp"
#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
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
        DefaultDeserializers::addDefault(_valueDeserializers);
        return *this;
    }

    INLINE ISettingsParser::Ptr CommandLineParserBuilder::build()
    {
        auto hadWhiteSpaceSplitters = tryRemoveWhiteSpaceSplitters();
        return std::make_unique<details::CommandLineParser>(
            getSplitter(), getValueDeserializersMap(), std::move(getConfig().prefixes), hadWhiteSpaceSplitters);
    }

    INLINE bool CommandLineParserBuilder::tryRemoveWhiteSpaceSplitters()
    {
        auto &splitters = getConfig().splitters;
        const auto it = details::utils::removeIf(splitters.begin(), splitters.end(),
                                                 [](auto splitter) { return details::utils::isWhiteSpace(splitter); });
        const auto removeCnt = splitters.end() - it;
        splitters.erase(it, splitters.end());
        return removeCnt;
    }

    INLINE ISettingSplitter::Ptr CommandLineParserBuilder::getSplitter()
    {
        if (!_splitter)
        {
            auto &config = getConfig();
            useSplitter(std::make_unique<details::SettingSplitter>(
                std::move(config.splitters), std::move(config.typeMarkers),
                std::move(config.keySplitters), config.allowEmptyKeys));
        }
        return std::move(_splitter);
    }

    INLINE IValueDeserializersMap::Ptr CommandLineParserBuilder::getValueDeserializersMap()
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

    INLINE CommandLineParserConfig &CommandLineParserBuilder::getConfig()
    {
        if (!_config)
        {
            useConfig({});
        }
        return *_config;
    }
} // namespace sb::cf
