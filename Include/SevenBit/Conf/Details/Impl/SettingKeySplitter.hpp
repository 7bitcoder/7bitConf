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
    INLINE SettingKeySplitter::SettingKeySplitter(std::vector<std::string_view> keySplitters)
        : _keySplitters(keySplitters)
    {
    }

    INLINE std::vector<std::string_view> SettingKeySplitter::split(std::string_view key) const
    {
        return utils::split(key, _keySplitters);
    }
} // namespace sb::cf::details