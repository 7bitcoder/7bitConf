#pragma once

#include <algorithm>

#include "SevenBit/Conf/Details/EnvironmentVarsParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    INLINE EnvironmentVarsParser::EnvironmentVarsParser(ISettingSplitter::Ptr settingSplitter,
                                                        IValueDeserializersMap::Ptr valueDeserializersMap)
        : _settingSplitter(std::move(settingSplitter)), _valueDeserializersMap(std::move(valueDeserializersMap))
    {
        utils::assertPtr(_settingSplitter);
        utils::assertPtr(_valueDeserializersMap);
    }

    INLINE JsonObject EnvironmentVarsParser::parse(const std::vector<std::string_view> &envVariables) const
    {
        JsonObject result;
        for (const auto variable : envVariables)
        {
            try
            {
                auto [keys, type, value] = _settingSplitter->split(variable);
                JsonExt::updateWith(result, keys, _valueDeserializersMap->getDeserializerFor(type).deserialize(value));
            }
            catch (const std::exception &e)
            {
                throw SettingParserException("Parsing error for environment variable '" + std::string{variable} +
                                             "' error: " + e.what());
            }
        }
        return result;
    }
} // namespace sb::cf::details
