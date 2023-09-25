#pragma once

#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IValueDeserializersMap.hpp"

namespace sb::cf::details
{
    class EXPORT ValueDeserializersMap : public IValueDeserializersMap
    {
      private:
        std::vector<std::pair<std::string, std::unique_ptr<IDeserializer>>> _deserializersLookup;

      public:
        ValueDeserializersMap(const ValueDeserializersMap &) = delete;
        
        ValueDeserializersMap &operator=(const ValueDeserializersMap &) = delete;

        using Ptr = std::unique_ptr<ValueDeserializersMap>;

        std::vector<std::pair<std::string, std::unique_ptr<IDeserializer>>> &getDeserializersLookup();

        void add(std::string_view type, std::unique_ptr<IDeserializer> deserializer);

        const IDeserializer *getDeserializerFor(std::string_view type) const override;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ValueDeserializers.hpp"
#endif