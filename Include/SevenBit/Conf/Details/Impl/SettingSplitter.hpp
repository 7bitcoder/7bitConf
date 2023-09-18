#pragma once
#include <cstddef>
#include <cstdint>
#include <tao/json/from_string.hpp>

#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    INLINE SettingSplitter::SettingSplitter(std::string_view settingPrefix, std::string_view settingSplitter)
        : _settingPrefix(settingPrefix), _settingSplitter(settingSplitter)
    {
    }

    INLINE SettingSplitter::Result SettingSplitter::split(std::string_view setting) const
    {
        auto splitted = details::utils::split(tryRemovePrefix(setting), _settingSplitter, 2);
        switch (splitted.size())
        {
        case 1:
            return {splitted[0]};
        case 2:
            return {splitted[0], splitted[1]};
        default:
            throw SettingParserException("Wrong setting format: " + std::string{setting} +
                                         " it should follow this scheme [--]setting[:nestedSetting]...[!type]=[value]");
        }
    }

    INLINE std::string_view SettingSplitter::tryRemovePrefix(std::string_view setting) const
    {
        if (details::utils::startsWith(setting, _settingPrefix))
        {
            setting.remove_prefix(_settingPrefix.size());
        }
        return setting;
    }

    INLINE bool operator==(const SettingSplitter::Result &lhs, const SettingSplitter::Result &rhs)
    {
        return lhs.key == rhs.key && lhs.value == rhs.value;
    }
} // namespace sb::cf::details