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
        auto splitted = utils::split(key, _typeMarkers, 2);
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

    INLINE bool operator==(const SettingTypeSplitter::Result &lhs, const SettingTypeSplitter::Result &rhs)
    {
        return lhs.key == rhs.key && lhs.type == rhs.type;
    }
} // namespace sb::cf::details