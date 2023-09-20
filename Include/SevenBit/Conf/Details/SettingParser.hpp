#pragma once

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/SettingSplitter.hpp"
#include "SevenBit/Conf/ISettingParser.hpp"
#include "SevenBit/Conf/IValueDeserializers.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf::details
{
    class EXPORT SettingParser : public ISettingParser
    {
      private:
        const ISettingSplitter::Ptr _settingSplitter;
        const IValueDeserializers::Ptr _valueDeserializers;

        const std::string_view _presumedType;
        const bool _allowEmptyKeys;
        const bool _throwOnUnknownType;

      public:
        using Ptr = std::unique_ptr<SettingParser>;

        static Ptr createDefault(SettingParserConfig config);

        SettingParser(ISettingSplitter::Ptr settingSplitter, IValueDeserializers::Ptr valueDeserializers,
                      std::string_view presumedType, bool allowEmptyKeys, bool throwOnUnknownType);

        ISettingParser::Result parse(std::string_view setting) const override;

      private:
        ISettingParser::Result getKeysAndValue(std::string_view setting) const;

        void checkKeys(const std::vector<std::string_view> &keys) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingParser.hpp"
#endif