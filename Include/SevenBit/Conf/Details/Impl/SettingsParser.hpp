#pragma once

#include <algorithm>

#include "SevenBit/Conf/Details/SettingsParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

#include <SevenBit/Conf/Details/JsonExt.hpp>

namespace sb::cf::details
{
    INLINE SettingsParser::SettingsParser(ISettingSplitter::Ptr settingSplitter,
                                          IValueDeserializersMap::Ptr valueDeserializersMap,
                                          std::vector<std::string_view> settingPrefixes, bool combinedSetting)
        : _settingSplitter(std::move(settingSplitter)), _valueDeserializersMap(std::move(valueDeserializersMap)),
          _settingPrefixes(std::move(settingPrefixes)), _combinedSetting(combinedSetting)
    {
        utils::assertPtr(_settingSplitter);
        utils::assertPtr(_valueDeserializersMap);
    }

    INLINE JsonObject SettingsParser::parse(const std::vector<std::string_view> &settings) const
    {
        JsonObject result;
        for (size_t index = 0; index < settings.size(); ++index)
        {
            try
            {
                auto [keys, value] = parseArgument(settings, index);
                JsonExt::updateWith(result, keys, std::move(value));
            }
            catch (const std::exception &e)
            {
                throw SettingParserException("Parsing error for setting '" + std::string{settings[index]} +
                                             "' error: " + e.what());
            }
        }
        return result;
    }

    INLINE SettingsParser::ArgumentParseResult SettingsParser::parseArgument(
        const std::vector<std::string_view> &settings, size_t &index) const
    {
        auto setting = settings[index];
        const auto hasOptionPrefix = tryRemovePrefix(setting);

        auto [keys, type, value] = _settingSplitter->split(setting);

        if (_combinedSetting && hasOptionPrefix && !value && index + 1 < settings.size() &&
            !tryGetPrefix(settings[index + 1]))
        {
            value = settings[++index];
        }
        return {keys, _valueDeserializersMap->getDeserializerFor(type).deserialize(value)};
    }

    INLINE bool SettingsParser::tryRemovePrefix(std::string_view &setting) const
    {
        if (const auto settingPrefix = tryGetPrefix(setting))
        {
            setting.remove_prefix(settingPrefix->size());
            return true;
        }
        return false;
    }

    INLINE std::optional<std::string_view> SettingsParser::tryGetPrefix(std::string_view setting) const
    {
        for (auto &settingPrefix : _settingPrefixes)
        {
            if (utils::startsWith(setting, settingPrefix))
            {
                return settingPrefix;
            }
        }
        return std::nullopt;
    }

} // namespace sb::cf::details
