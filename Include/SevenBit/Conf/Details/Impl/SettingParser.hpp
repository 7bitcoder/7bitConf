#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(SettingParserConfig config)
        : _config(std::move(config)), _settingSplitter(_config.settingPrefixes, _config.settingSplitters),
          _settingKeySplitter(_config.keySplitters), _settingTypeSplitter(_config.typeMarkers)
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
            auto [keys, value] = getKeysAndValue(setting);
            JsonExt::updateWith(result, keys, std::move(value));
        }
        catch (const std::exception &e)
        {
            throw SettingParserException{"Error for setting: '" + std::string{setting} + " ' error: " + e.what()};
        }
    }

    SettingParser::KeysAndValue SettingParser::getKeysAndValue(std::string_view setting) const
    {
        auto [keys, type, value] = getKeysTypeAndValue(setting);

        checkKeys(keys);

        if (auto deserializer = _deserializers.getDeserializer(type ? *type : _config.preasumedType))
        {
            return {keys, deserializer->deserialize(value)};
        }
        if (_config.throwOnUnknownType)
        {
            // todo throw
        }
    }

    SettingParser::KeysTypeAndValue SettingParser::getKeysTypeAndValue(std::string_view setting) const
    {
        auto [key, value] = _settingSplitter.split(setting);
        auto [rawKey, type] = _settingTypeSplitter.split(key);
        return {_settingKeySplitter.split(rawKey), type, value};
    }

    void SettingParser::checkKeys(const std::vector<std::string_view> &keys) const
    {
        if (_config.allowEmptyKeys)
        {
            return;
        }
        for (const auto &key : keys)
        {
            if (key.empty())
            {
                // todo throw
            }
        }
    }
} // namespace sb::cf::details
