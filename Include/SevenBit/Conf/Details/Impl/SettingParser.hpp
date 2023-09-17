#pragma once

#include <cstddef>
#include <cstdint>

#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(SettingParserConfig config)
        : _config(config), _settingSplitter(config), _settingKeySplitter(config)
    {
    }

    INLINE JsonObject SettingParser::parse(std::string_view setting) const
    {
        JsonObject result;
        parseInto(setting, result);
        return result;
    }

    INLINE void SettingParser::parseInto(std::string_view setting, JsonObject &result) const
    {
        try
        {
            auto [key, value] = _settingSplitter.split(setting);
            auto [keys, type] = _settingKeySplitter.split(key);

            auto &deserializer = _deserializers.getDeserializer(type);
            JsonExt::updateWith(result, keys, deserializer.deserialize(value));
        }
        catch (const std::exception &e)
        {
            throw SettingParserException{"Error for setting: '" + std::string{setting} + " ' error: " + e.what()};
        }
    }
} // namespace sb::cf::details
