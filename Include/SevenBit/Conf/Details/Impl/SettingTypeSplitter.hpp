#pragma once
#include <cstddef>
#include <cstdint>
#include <tao/json/from_string.hpp>

#include "SevenBit/Conf/Details/SettingTypeSplitter.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    INLINE SettingTypeSplitter::SettingTypeSplitter(std::vector<std::string_view> typeMarkers)
        : _typeMarkers(typeMarkers)
    {
    }

    INLINE SettingTypeSplitter::Result SettingTypeSplitter::split(std::string_view key) const
    {
        auto splitted = utils::split(checkAndPrepareKey(key), _typeMarkers, 2);
        switch (splitted.size())
        {
        case 1:
            return {splitted[0]};
        case 2:
            return {splitted[0], splitted[1]};
        default:
            throw std::runtime_error("invalid split");
        }
    }

    INLINE std::size_t SettingTypeSplitter::tryFindDividersAt(std::string_view key, size_t index,
                                                              std::string_view divider,
                                                              std::string_view alternativeDivider) const
    {
        auto size = tryFindDividerAt(key, index, divider);
        return size ? size : tryFindDividerAt(key, index, alternativeDivider);
    }

    INLINE std::size_t SettingTypeSplitter::tryFindDividerAt(std::string_view key, size_t index,
                                                             std::string_view divider) const
    {
        return details::utils::backwardContainsAt(key, index, divider) ? divider.size() : 0;
    }

    INLINE std::string_view SettingTypeSplitter::extractElement(std::string_view &key, size_t &index,
                                                                size_t dividerSize) const
    {
        auto element = key.substr(index + 1);
        key.remove_suffix(dividerSize + element.size());
        index = key.size();
        return element;
    }

    INLINE bool operator==(const SettingTypeSplitter::Result &lhs, const SettingTypeSplitter::Result &rhs)
    {
        return lhs.keys == rhs.keys && lhs.type == rhs.type;
    }
} // namespace sb::cf::details