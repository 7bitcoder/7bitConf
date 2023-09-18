#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf::details
{
    class EXPORT SettingSplitter
    {
      private:
        std::string_view _settingSplitter;

      public:
        struct Result
        {
            std::string_view key;
            std::optional<std::string_view> value;
        };

        SettingSplitter(std::string_view settingSplitter);

        Result split(std::string_view setting) const;
    };

    bool operator==(const SettingSplitter::Result &lhs, const SettingSplitter::Result &rhs);
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingSplitter.hpp"
#endif