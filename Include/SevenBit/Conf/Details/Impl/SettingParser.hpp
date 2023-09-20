#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Details/ValueDeserializers.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(ISettingSplitter::Ptr settingSplitter,
                                        IValueDeserializers::Ptr valueDeserializers, std::string_view presumedType,
                                        bool allowEmptyKeys, bool throwOnUnknownType)
        : _settingSplitter(std::move(settingSplitter)), _valueDeserializers(std::move(valueDeserializers)),
          _presumedType(presumedType), _allowEmptyKeys(allowEmptyKeys), _throwOnUnknownType(throwOnUnknownType)
    {
    }

    INLINE ISettingParser::Result SettingParser::parse(std::string_view setting) const
    {
        try
        {
            return getKeysAndValue(setting);
        }
        catch (const std::exception &e)
        {
            throw SettingParserException("Parsing error for setting '" + std::string{setting} + "' error: " + e.what());
        }
    }

    INLINE ISettingParser::Result SettingParser::getKeysAndValue(std::string_view setting) const
    {
        auto [keys, type, value] = _settingSplitter->split(setting);

        checkKeys(keys);
        auto finalType = type ? *type : _presumedType;

        auto deserializer = _valueDeserializers->getDeserializerFor(finalType);
        if (!deserializer)
        {
            if (_throwOnUnknownType)
            {
                throw std::runtime_error("Unknown type: " + std::string{finalType});
            }
            deserializer = &_valueDeserializers->getDefaultDeserializer();
        }
        return {std::move(keys), deserializer->deserialize(value)};
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
