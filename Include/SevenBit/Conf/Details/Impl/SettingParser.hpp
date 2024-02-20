#pragma once

#include <algorithm>

#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(ISettingSplitter::Ptr settingSplitter,
                                        IValueDeserializersMap::Ptr valueDeserializersMap)
        : _settingSplitter(std::move(settingSplitter)), _valueDeserializersMap(std::move(valueDeserializersMap))
    {
        utils::assertPtr(_settingSplitter);
        utils::assertPtr(_valueDeserializersMap);
    }

    INLINE ISettingParser::Result SettingParser::parse(std::string_view setting) const
    {
        try
        {
            auto [keys, type, value] = getSettingSplitter().split(setting);
            return {std::move(keys), getValueDeserializersMap().getDeserializerFor(type).deserialize(value)};
        }
        catch (const std::exception &e)
        {
            throw SettingParserException("Parsing error for setting '" + std::string{setting} + "' error: " + e.what());
        }
    }

    INLINE const ISettingSplitter &SettingParser::getSettingSplitter() const { return *_settingSplitter; }

    INLINE const IValueDeserializersMap &SettingParser::getValueDeserializersMap() const
    {
        return *_valueDeserializersMap;
    }
} // namespace sb::cf::details
