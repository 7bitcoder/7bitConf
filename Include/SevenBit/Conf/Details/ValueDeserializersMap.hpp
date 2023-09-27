#pragma once

#include <map>
#include <string>
#include <string_view>
#include <utility>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IValueDeserializersMap.hpp"

namespace sb::cf::details
{
    class EXPORT ValueDeserializersMap : public IValueDeserializersMap
    {
      private:
        struct CaseInsensitiveLess
        {
            using is_transparent = int;

            template <class T1, class T2> bool operator()(const T1 &s1, const T2 &s2) const
            {
                return utils::ignoreCaseLess(s1, s2);
            }
        };

        std::map<std::string, IDeserializer::Ptr, CaseInsensitiveLess> _deserializersLookup;

      public:
        using Ptr = std::unique_ptr<ValueDeserializersMap>;

        ValueDeserializersMap() = default;

        ValueDeserializersMap(const ValueDeserializersMap &) = delete;
        ValueDeserializersMap(ValueDeserializersMap &&) noexcept = default;

        ValueDeserializersMap &operator=(const ValueDeserializersMap &) = delete;
        ValueDeserializersMap &operator=(ValueDeserializersMap &&) = default;

        [[nodiscard]] std::map<std::string, IDeserializer::Ptr, CaseInsensitiveLess> &getDeserializersMap();

        void add(std::string_view type, IDeserializer::Ptr deserializer);

        [[nodiscard]] const IDeserializer *getDeserializerFor(std::string_view type) const override;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ValueDeserializersMap.hpp"
#endif