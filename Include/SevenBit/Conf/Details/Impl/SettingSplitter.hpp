#pragma once

#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/StringUtils.hpp"

namespace sb::cf::details
{

    INLINE SettingSplitter::SettingSplitter(std::vector<std::string_view> settingSplitters,
                                            std::vector<std::string_view> typeMarkers,
                                            std::vector<std::string_view> keySplitters, const bool allowEmptyKeys)
        : _settingSplitters(std::move(settingSplitters)), _typeMarkers(std::move(typeMarkers)),
          _keySplitters(std::move(keySplitters)), _allowEmptyKeys(allowEmptyKeys)
    {
    }

    INLINE ISettingSplitter::Result SettingSplitter::split(const std::string_view setting) const
    {
        auto [rawKey, value] = splitSetting(setting);
        const auto type = tryExtractType(rawKey);
        return {splitKey(rawKey), type, value};
    }

    INLINE const std::vector<std::string_view> &SettingSplitter::getSettingSplitters() const
    {
        return _settingSplitters;
    }

    INLINE const std::vector<std::string_view> &SettingSplitter::getTypeMarkers() const { return _typeMarkers; }

    INLINE const std::vector<std::string_view> &SettingSplitter::getKeySplitters() const { return _keySplitters; }

    INLINE bool SettingSplitter::getAllowEmptyKeys() const { return _allowEmptyKeys; }

    INLINE std::pair<std::string_view, std::optional<std::string_view>> SettingSplitter::splitSetting(
        const std::string_view setting) const
    {
        if (auto breakResult = StringUtils::tryBreak(setting, getSettingSplitters()))
        {
            return {breakResult->first, breakResult->second};
        }
        return {setting, std::nullopt};
    }

    INLINE std::optional<std::string_view> SettingSplitter::tryExtractType(std::string_view &key) const
    {
        if (auto breakResult = StringUtils::tryBreakFromEnd(key, getTypeMarkers()))
        {
            key = breakResult->first;
            return breakResult->second;
        }
        return std::nullopt;
    }

    INLINE std::vector<std::string_view> SettingSplitter::splitKey(const std::string_view key) const
    {
        auto keys = StringUtils::split(key, getKeySplitters());
        checkKeys(keys);
        return keys;
    }

    INLINE void SettingSplitter::checkKeys(const std::vector<std::string_view> &keys) const
    {
        if (getAllowEmptyKeys())
        {
            return;
        }
        if (keys.empty() ||
            std::any_of(keys.begin(), keys.end(), [](const std::string_view &key) { return key.empty(); }))
        {
            throw ConfigException("Setting key cannot be empty");
        }
    }
} // namespace sb::cf::details
