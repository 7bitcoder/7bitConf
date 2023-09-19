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
    INLINE SettingSplitter::SettingSplitter(std::vector<std::string_view> settingPrefixes,
                                            std::vector<std::string_view> settingSplitters)
        : _settingPrefixes(settingPrefixes), _settingSplitters(settingSplitters)
    {
    }

    INLINE SettingSplitter::Result SettingSplitter::split(std::string_view setting) const
    {
        auto splitted = details::utils::split(tryRemovePrefix(setting), _settingSplitters, 2);
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

    INLINE bool operator==(const SettingSplitter::Result &lhs, const SettingSplitter::Result &rhs)
    {
        return lhs.key == rhs.key && lhs.value == rhs.value;
    }
} // namespace sb::cf::details