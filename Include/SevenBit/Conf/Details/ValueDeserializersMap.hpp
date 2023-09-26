#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IValueDeserializersMap.hpp"

namespace sb::cf::details
{
    class EXPORT ValueDeserializersMap : public IValueDeserializersMap
    {
      private:
        std::vector<std::pair<std::string, IDeserializer::Ptr>> _deserializersLookup;

      public:
        using Ptr = std::unique_ptr<ValueDeserializersMap>;

        ValueDeserializersMap() = default;

        ValueDeserializersMap(const ValueDeserializersMap &) = delete;
        ValueDeserializersMap(ValueDeserializersMap &&) = default;

        ValueDeserializersMap &operator=(const ValueDeserializersMap &) = delete;
        ValueDeserializersMap &operator=(ValueDeserializersMap &&) = default;

        std::vector<std::pair<std::string, IDeserializer::Ptr>> &getDeserializersLookup();

        void add(std::string_view type, IDeserializer::Ptr deserializer);

        const IDeserializer *getDeserializerFor(std::string_view type) const override;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ValueDeserializers.hpp"
#endif