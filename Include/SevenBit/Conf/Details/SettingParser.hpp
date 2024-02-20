#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ISettingParser.hpp"
#include "SevenBit/Conf/ISettingSplitter.hpp"
#include "SevenBit/Conf/IValueDeserializersMap.hpp"

namespace sb::cf::details
{
    class EXPORT SettingParser : public ISettingParser
    {
      private:
        const ISettingSplitter::Ptr _settingSplitter;
        const IValueDeserializersMap::Ptr _valueDeserializersMap;

      public:
        using Ptr = std::unique_ptr<SettingParser>;

        SettingParser(ISettingSplitter::Ptr settingSplitter, IValueDeserializersMap::Ptr valueDeserializersMap);

        [[nodiscard]] ISettingParser::Result parse(std::string_view setting) const override;

        [[nodiscard]] const ISettingSplitter &getSettingSplitter() const;

        [[nodiscard]] const IValueDeserializersMap &getValueDeserializersMap() const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif
