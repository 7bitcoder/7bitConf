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
    INLINE SettingSplitter::SettingSplitter(SettingParserConfig cfg) : _config(cfg) {}

    INLINE SettingSplitter::Result SettingSplitter::split(std::string_view setting) const
    {
        auto splitted = details::utils::split(setting, _config.settingSplitter, 2);
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

    INLINE bool operator==(const SettingSplitter::Result &lhs, const SettingSplitter::Result &rhs)
    {
        return lhs.key == rhs.key && lhs.value == rhs.value;
    }
} // namespace sb::cf::details