#pragma once
#include <cstddef>
#include <cstdint>
#include <tao/json/from_string.hpp>
#include <utility>

#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

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
        auto splitted = details::utils::split(setting, _settingSplitters, 2);
        switch (splitted.size())
        {
        case 1:
            return {splitted[0], std::nullopt};
        case 2:
            return {splitted[0], splitted[1]};
        default:
            throwWrongFormatError("Only one type setting splitter is allowed");
        }
    }

    INLINE std::pair<std::string_view, std::optional<std::string_view>> SettingSplitter::splitType(
        std::string_view key) const
    {
        auto splitted = details::utils::backwardsSplit(key, _typeMarkers, 2);
        switch (splitted.size())
        {
        case 1:
            return {splitted[0], std::nullopt};
        case 2:
            return {splitted[0], splitted[1]};
        default:
            throwWrongFormatError("Only one type marker is allowed");
        }
    }

    INLINE std::vector<std::string_view> SettingSplitter::splitKey(std::string_view key) const
    {
        return details::utils::split(key, _keySplitters);
    }

    INLINE void SettingSplitter::throwWrongFormatError(const std::string &what) const
    {
        throw std::runtime_error(
            what + ", wrong setting format it should follow this scheme [--]setting[:nestedSetting]...[!type]=[value]");
    }
} // namespace sb::cf::details