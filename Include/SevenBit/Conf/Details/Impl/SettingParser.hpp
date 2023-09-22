#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(ISettingSplitter::Ptr settingSplitter,
                                        IValueDeserializersMap::Ptr valueDeserializersMap, std::string_view defaultType,
                                        bool allowEmptyKeys, bool throwOnUnknownType)
        : _settingSplitter(std::move(settingSplitter)), _valueDeserializersMap(std::move(valueDeserializersMap)),
          _defaultType(defaultType), _allowEmptyKeys(allowEmptyKeys), _throwOnUnknownType(throwOnUnknownType)
    {
    }

    INLINE ISettingParser::Result SettingParser::parse(std::string_view setting) const
    {
        try
        {
            auto [keys, type, value] = _settingSplitter->split(setting);

            checkKeys(keys);

            return {std::move(keys), getDeserializerFor(type ? *type : _defaultType).deserialize(value)};
        }
        catch (const std::exception &e)
        {
            throw SettingParserException("Parsing error for setting '" + std::string{setting} + "' error: " + e.what());
        }
    }

    INLINE const IDeserializer &SettingParser::getDeserializerFor(std::string_view type) const
    {
        auto deserializer = _valueDeserializersMap->getDeserializerFor(type);
        if (!deserializer)
        {
            if (_throwOnUnknownType)
            {
                throw std::runtime_error("Unknown setting type: " + std::string{type});
            }
            deserializer = _valueDeserializersMap->getDeserializerFor(_defaultType);
            if (!deserializer)
            {
                throw std::runtime_error("Unknown default setting type: " + std::string{_defaultType});
            }
        }
        return *deserializer;
    }

    INLINE void SettingParser::checkKeys(const std::vector<std::string_view> &keys) const
    {
        if (_allowEmptyKeys)
        {
            return;
        }
        if (keys.empty() || std::any_of(keys.begin(), keys.end(), [](auto key) { return key.empty(); }))
        {
            throw std::runtime_error("Wrong setting format: empty key");
        }
    }
} // namespace sb::cf::details
