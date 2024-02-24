#pragma once

#include <algorithm>

#include "SevenBit/Conf/Details/EnvironmentVarsParser.hpp"
#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/Require.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    INLINE EnvironmentVarsParser::EnvironmentVarsParser(ISettingSplitter::Ptr settingSplitter,
                                                        IValueDeserializersMap::Ptr valueDeserializersMap)
        : _settingSplitter(std::move(settingSplitter)), _valueDeserializersMap(std::move(valueDeserializersMap))
    {
        Require::notNull(_settingSplitter);
        Require::notNull(_valueDeserializersMap);
    }

    INLINE JsonObject EnvironmentVarsParser::parse(const std::vector<std::string_view> &envVariables) const
    {
        JsonObject result;
        for (const auto variable : envVariables)
        {
            try
            {
                auto [keys, type, value] = getSettingSplitter().split(variable);
                JsonExt::updateWith(result, keys,
                                    getValueDeserializersMap().getDeserializerFor(type).deserialize(value));
            }
            catch (const std::exception &e)
            {
                throw SettingParserException("Parsing error for environment variable '" + std::string{variable} +
                                             "' error: " + e.what());
            }
        }
        return result;
    }

    INLINE const ISettingSplitter &EnvironmentVarsParser::getSettingSplitter() const { return *_settingSplitter; }

    INLINE const IValueDeserializersMap &EnvironmentVarsParser::getValueDeserializersMap() const
    {
        return *_valueDeserializersMap;
    }
} // namespace sb::cf::details
