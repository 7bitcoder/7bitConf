#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(ISettingSplitter::Ptr settingSplitter,
                                        IValueDeserializersMap::Ptr valueDeserializersMap, std::string_view defaultType,
                                        bool allowEmptyKeys, bool throwOnUnknownType)
        : _settingSplitter(std::move(settingSplitter)), _valueDeserializersMap(std::move(valueDeserializersMap)),
          _defaultType(defaultType), _allowEmptyKeys(allowEmptyKeys), _throwOnUnknownType(throwOnUnknownType)
    {
        if (!_settingSplitter)
        {
            throw NullPointnerException("Setting splitter cannot be null");
        }
        if (!_valueDeserializersMap)
        {
            throw NullPointnerException("Value deserializer map cannot be null");
        }
    }

    INLINE ISettingParser::Result SettingParser::parse(std::string_view setting) const
    {
        try
        {
            auto [keys, type, value] = getSettingSplitter().split(setting);

            checkKeys(keys);

            return {std::move(keys), getDeserializerFor(type ? *type : getDefaultType()).deserialize(value)};
        }
        catch (const std::exception &e)
        {
            throw SettingParserException("Parsing error for setting '" + std::string{setting} + "' error: " + e.what());
        }
    }

    INLINE const IDeserializer &SettingParser::getDeserializerFor(std::string_view type) const
    {
        auto deserializer = getValueDeserializersMap().getDeserializerFor(type);
        if (!deserializer)
        {
            if (getThrowOnUnknownType())
            {
                throw ConfigException("Unknown setting type: " + std::string{type});
            }
            deserializer = getValueDeserializersMap().getDeserializerFor(getDefaultType());
            if (!deserializer)
            {
                throw ConfigException("Unknown default setting type: " + std::string{getDefaultType()});
            }
        }
        return *deserializer;
    }

    INLINE void SettingParser::checkKeys(const std::vector<std::string_view> &keys) const
    {
        if (getAllowEmptyKeys())
        {
            return;
        }
        if (keys.empty() || std::any_of(keys.begin(), keys.end(), [](auto key) { return key.empty(); }))
        {
            throw ConfigException("Setting key cannot be empty");
        }
    }

    INLINE const ISettingSplitter &SettingParser::getSettingSplitter() const { return *_settingSplitter; }

    INLINE const IValueDeserializersMap &SettingParser::getValueDeserializersMap() const
    {
        return *_valueDeserializersMap;
    }

    INLINE std::string_view SettingParser::getDefaultType() const { return _defaultType; }

    INLINE bool SettingParser::getAllowEmptyKeys() const { return _allowEmptyKeys; }

    INLINE bool SettingParser::getThrowOnUnknownType() const { return _throwOnUnknownType; }

} // namespace sb::cf::details
