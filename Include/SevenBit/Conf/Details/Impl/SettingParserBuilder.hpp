#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/ValueDeserializers.hpp"
#include "SevenBit/Conf/SettingParserBuilder.hpp"

namespace sb::cf
{
    INLINE SettingParserBuilder &SettingParserBuilder::useSplitter(ISettingSplitter::Ptr splitter)
    {
        _splitter = std::move(splitter);
        return *this;
    }

    INLINE SettingParserBuilder &SettingParserBuilder::useValueDeserializers(
        IValueDeserializers::Ptr valueDeserializers)
    {
        _valueDeserializers = std::move(valueDeserializers);
        return *this;
    }

    INLINE SettingParserBuilder &SettingParserBuilder::useDeserializerFor(std::string type,
                                                                          IDeserializer::Ptr deserializer)
    {
        _deserializersMap.emplace_back(std::move(type), std::move(deserializer));
        return *this;
    }

    INLINE SettingParserBuilder &SettingParserBuilder::useConfig(SettingParserConfig config)
    {
        _config = std::move(config);
        return *this;
    }

    INLINE ISettingParser::Ptr SettingParserBuilder::build()
    {
        auto &config = getConfig();
        return std::make_unique<details::SettingParser>(getSplitter(), getValueDeserializers(), config.presumedType,
                                                        config.allowEmptyKeys, config.throwOnUnknownType);
    }

    INLINE ISettingSplitter::Ptr SettingParserBuilder::getSplitter()
    {
        if (_splitter)
        {
            return std::move(_splitter);
        }
        auto &config = getConfig();
        return std::make_unique<details::SettingSplitter>(
            std::move(config.settingPrefixes), std::move(config.settingSplitters), std::move(config.typeMarkers),
            std::move(config.keySplitters));
    }

    INLINE IValueDeserializers::Ptr SettingParserBuilder::getValueDeserializers()
    {
        if (_valueDeserializers)
        {
            return std::move(_valueDeserializers);
        }
        if (_deserializersMap.empty())
        {
            _deserializersMap.emplace_back("string", std::make_unique<details::StringDeserializer>());
            _deserializersMap.emplace_back("bool", std::make_unique<details::BoolDeserializer>());
            _deserializersMap.emplace_back("int", std::make_unique<details::IntDeserializer>());
            _deserializersMap.emplace_back("double", std::make_unique<details::DoubleDeserializer>());
            _deserializersMap.emplace_back("uint", std::make_unique<details::UIntDeserializer>());
            _deserializersMap.emplace_back("json", std::make_unique<details::JsonDeserializer>());
            _deserializersMap.emplace_back("null", std::make_unique<details::NullDeserializer>());
        }
        auto valueDeserializers = std::make_unique<details::ValueDeserializers>();
        for (auto &[type, deserializer] : _deserializersMap)
        {
            valueDeserializers->add(std::move(type), std::move(deserializer));
        }
        return valueDeserializers;
    }

    INLINE SettingParserConfig &SettingParserBuilder::getConfig()
    {
        if (!_config)
        {
            _config = SettingParserConfig{};
        }
        return *_config;
    }
} // namespace sb::cf
