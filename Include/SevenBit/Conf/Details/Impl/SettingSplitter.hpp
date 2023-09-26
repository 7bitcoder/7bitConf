#pragma once

#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"

namespace sb::cf::details
{

    INLINE SettingSplitter::SettingSplitter(std::vector<std::string_view> settingPrefixes,
                                            std::vector<std::string_view> settingSplitters,
                                            std::vector<std::string_view> typeMarkers,
                                            std::vector<std::string_view> keySplitters)
        : _settingPrefixes(std::move(settingPrefixes)), _settingSplitters(std::move(settingSplitters)),
          _typeMarkers(std::move(typeMarkers)), _keySplitters(std::move(keySplitters))
    {
    }

    INLINE ISettingSplitter::Result SettingSplitter::split(std::string_view setting) const
    {
        auto [key, value] = splitSetting(tryRemovePrefix(setting));
        auto [rawKey, type] = splitType(key);
        return {splitKey(rawKey), type, value};
    }

    INLINE std::string_view SettingSplitter::tryRemovePrefix(std::string_view setting) const
    {
        for (auto &prefix : _settingPrefixes)
        {
            if (details::utils::startsWith(setting, prefix))
            {
                setting.remove_prefix(prefix.size());
                break;
            }
        }
        return setting;
    }

    INLINE std::pair<std::string_view, std::optional<std::string_view>> SettingSplitter::splitSetting(
        std::string_view setting) const
    {
        if (auto breakResult = details::utils::tryBreak(setting, _settingSplitters))
        {
            return {breakResult->first, breakResult->second};
        }
        return {setting, std::nullopt};
    }

    INLINE std::pair<std::string_view, std::optional<std::string_view>> SettingSplitter::splitType(
        std::string_view key) const
    {
        if (auto breakResult = details::utils::tryBreakFromEnd(key, _typeMarkers))
        {
            return {breakResult->first, breakResult->second};
        }
        return {key, std::nullopt};
    }

    INLINE std::vector<std::string_view> SettingSplitter::splitKey(std::string_view key) const
    {
        return details::utils::split(key, _keySplitters);
    }
} // namespace sb::cf::details