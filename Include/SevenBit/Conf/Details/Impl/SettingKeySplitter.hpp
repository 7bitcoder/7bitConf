#pragma once
#include <cstddef>
#include <cstdint>
#include <tao/json/from_string.hpp>

#include "SevenBit/Conf/Details/SettingKeySplitter.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    INLINE SettingKeySplitter::SettingKeySplitter(SettingParserConfig cfg) : _config(cfg) {}

    INLINE SettingKeySplitter::Result SettingKeySplitter::split(std::string_view key) const
    {
        key = checkAndPrepareKey(key);
        std::optional<std::string_view> type;
        std::vector<std::string_view> keys;
        std::size_t i = key.size(), dividerSize = 0;
        do
        {
            --i;
            auto canFindTypeMark = keys.empty() && !type;
            if (canFindTypeMark &&
                (dividerSize = tryFindDividersAt(key, i, _config.typeMarker, _config.alternativeTypeMarker)))
            {
                type = extractElement(key, i, dividerSize);
            }
            else if ((dividerSize = tryFindDividersAt(key, i, _config.keySplitter, _config.alternativeKeySplitter)))
            {
                keys.emplace_back(extractElement(key, i, dividerSize));
            }
        } while (i);
        keys.emplace_back(key);
        return {{keys.rbegin(), keys.rend()}, type ? *type : ""};
    }

    INLINE std::string_view SettingKeySplitter::checkAndPrepareKey(std::string_view key) const
    {
        if (details::utils::startsWith(key, _config.settingPrefix))
        {
            key.remove_prefix(_config.settingPrefix.size());
        }
        if (key.empty())
        {
            throw SettingParserException{"Key cannot be empty"};
        }
        return key;
    }

    INLINE std::size_t SettingKeySplitter::tryFindDividersAt(std::string_view key, size_t index,
                                                             std::string_view divider,
                                                             std::string_view alternativeDivider) const
    {
        auto size = tryFindDividerAt(key, index, divider);
        return size ? size : tryFindDividerAt(key, index, alternativeDivider);
    }

    INLINE std::size_t SettingKeySplitter::tryFindDividerAt(std::string_view key, size_t index,
                                                            std::string_view divider) const
    {
        return details::utils::backwardContainsAt(key, index, divider) ? divider.size() : 0;
    }

    INLINE std::string_view SettingKeySplitter::extractElement(std::string_view &key, size_t &index,
                                                               size_t dividerSize) const
    {
        auto element = key.substr(index + 1);
        key.remove_suffix(dividerSize + element.size());
        index = key.size();
        return element;
    }

    INLINE bool operator==(const SettingKeySplitter::Result &lhs, const SettingKeySplitter::Result &rhs)
    {
        return lhs.keys == rhs.keys && lhs.type == rhs.type;
    }
} // namespace sb::cf::details