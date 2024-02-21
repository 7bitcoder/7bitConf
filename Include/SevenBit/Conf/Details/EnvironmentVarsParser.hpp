#pragma once

#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ISettingSplitter.hpp"
#include "SevenBit/Conf/ISettingsParser.hpp"
#include "SevenBit/Conf/IValueDeserializersMap.hpp"

namespace sb::cf::details
{
    class EXPORT EnvironmentVarsParser : public ISettingsParser
    {
      private:
        const ISettingSplitter::Ptr _settingSplitter;
        const IValueDeserializersMap::Ptr _valueDeserializersMap;

      public:
        using Ptr = std::unique_ptr<EnvironmentVarsParser>;

        EnvironmentVarsParser(ISettingSplitter::Ptr settingSplitter, IValueDeserializersMap::Ptr valueDeserializersMap);

        [[nodiscard]] JsonObject parse(const std::vector<std::string_view> &envVariables) const override;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/EnvironmentVarsParser.hpp"
#endif
