#pragma once

#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IValueDeserializers.hpp"

namespace sb::cf::details
{
    class EXPORT ValueDeserializers : public IValueDeserializers
    {
      private:
        std::vector<std::pair<std::string, std::unique_ptr<IDeserializer>>> _deserializersLookup;

      public:
        using Ptr = std::unique_ptr<ValueDeserializers>;

        std::vector<std::pair<std::string, std::unique_ptr<IDeserializer>>> &getDeserializersLookup();

        void add(std::string_view type, std::unique_ptr<IDeserializer> deserializer);

        const IDeserializer *getDeserializerFor(std::string_view type) const override;

        const IDeserializer &getDefaultDeserializer() const override;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ValueDeserializers.hpp"
#endif