#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/IJsonDeserializer.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf::details
{
    class EXPORT SettingKeySplitter
    {
      private:
        const std::vector<std::string_view> _keySplitters;

      public:
        SettingKeySplitter(std::vector<std::string_view> keySplitters);

        std::vector<std::string_view> split(std::string_view key) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingKeySplitter.hpp"
#endif