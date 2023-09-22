#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/SettingParserBuilder.hpp"

namespace sb::cf
{
    INLINE SettingParserBuilder &SettingParserBuilder::useSplitter(ISettingSplitter::Ptr splitter)
    {
        _splitter = std::move(splitter);
        return *this;
    }

    INLINE SettingParserBuilder &SettingParserBuilder::useValueDeserializersMap(
        IValueDeserializersMap::Ptr valueDeserializersMap)
    {
        _valueDeserializersMap = std::move(valueDeserializersMap);
        return *this;
    }

    INLINE SettingParserBuilder &SettingParserBuilder::useValueDeserializer(std::string_view type,
                                                                            IDeserializer::Ptr valueDeserializer)
    {
        _deserializersMap.emplace_back(type, std::move(valueDeserializer));
        return *this;
    }

    INLINE SettingParserBuilder &SettingParserBuilder::useConfig(SettingParserConfig config)
    {
        _config = std::move(config);
        return *this;
    }

    INLINE SettingParserBuilder &SettingParserBuilder::useDefaultValueDeserializers()
    {
        useValueDeserializer("string", std::make_unique<details::StringDeserializer>());
        useValueDeserializer("bool", std::make_unique<details::BoolDeserializer>());
        useValueDeserializer("int", std::make_unique<details::IntDeserializer>());
        useValueDeserializer("double", std::make_unique<details::DoubleDeserializer>());
        useValueDeserializer("uint", std::make_unique<details::UIntDeserializer>());
        useValueDeserializer("json", std::make_unique<details::JsonDeserializer>());
        useValueDeserializer("null", std::make_unique<details::NullDeserializer>());
        return *this;
    }

    INLINE ISettingParser::Ptr SettingParserBuilder::build()
    {
        auto &config = getConfig();
        return std::make_unique<details::SettingParser>(getSplitter(), getValueDeserializersMap(), config.defaultType,
                                                        config.allowEmptyKeys, config.throwOnUnknownType);
    }

    INLINE ISettingSplitter::Ptr SettingParserBuilder::getSplitter()
    {
        if (!_splitter)
        {
            auto &config = getConfig();
            useSplitter(std::make_unique<details::SettingSplitter>(
                std::move(config.settingPrefixes), std::move(config.settingSplitters), std::move(config.typeMarkers),
                std::move(config.keySplitters)));
        }
        return std::move(_splitter);
    }

    INLINE IValueDeserializersMap::Ptr SettingParserBuilder::getValueDeserializersMap()
    {
        if (!_valueDeserializersMap)
        {
            if (_deserializersMap.empty())
            {
                useDefaultValueDeserializers();
            }
            auto valueDeserializers = std::make_unique<details::ValueDeserializersMap>();
            for (auto &[type, deserializer] : _deserializersMap)
            {
                valueDeserializers->add(std::move(type), std::move(deserializer));
            }
            useValueDeserializersMap(std::move(valueDeserializers));
        }
        return std::move(_valueDeserializersMap);
    }

    INLINE SettingParserConfig &SettingParserBuilder::getConfig()
    {
        if (!_config)
        {
            useConfig({});
        }
        return *_config;
    }
} // namespace sb::cf
