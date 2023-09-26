#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ISettingParser.hpp"
#include "SevenBit/Conf/ISettingSplitter.hpp"
#include "SevenBit/Conf/IValueDeserializersMap.hpp"
#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf::details
{
    class EXPORT SettingParser : public ISettingParser
    {
      private:
        const ISettingSplitter::Ptr _settingSplitter;
        const IValueDeserializersMap::Ptr _valueDeserializersMap;

        const std::string_view _defaultType;
        const bool _allowEmptyKeys;
        const bool _throwOnUnknownType;

      public:
        using Ptr = std::unique_ptr<SettingParser>;

        SettingParser(ISettingSplitter::Ptr settingSplitter, IValueDeserializersMap::Ptr valueDeserializersMap,
                      std::string_view defaultType, bool allowEmptyKeys, bool throwOnUnknownType);

        ISettingParser::Result parse(std::string_view setting) const override;

        const ISettingSplitter &getSettingSplitter() const;

        const IValueDeserializersMap &getValueDeserializersMap() const;

        std::string_view getDefaultType() const;

        bool getAllowEmptyKeys() const;

        bool getThrowOnUnknownType() const;

      private:
        const IDeserializer &getDeserializerFor(std::string_view type) const;

        void checkKeys(const std::vector<std::string_view> &keys) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif