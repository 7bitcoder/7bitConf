#pragma once

#include <map>
#include <string>
#include <string_view>
#include <utility>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/IValueDeserializersMap.hpp"

namespace sb::cf::details
{
    class EXPORT ValueDeserializersMap : public IValueDeserializersMap
    {
      public:
        struct CaseInsensitiveLess
        {
            using is_transparent = int;

            template <class T1, class T2> bool operator()(const T1 &s1, const T2 &s2) const
            {
                return utils::ignoreCaseLess(s1, s2);
            }
        };

        using Deserializers = std::vector<std::pair<std::string_view, IDeserializer::Ptr>>;
        using DeserializersMap = std::map<std::string, IDeserializer::Ptr, CaseInsensitiveLess>;

      private:
        DeserializersMap _deserializersMap;
        const std::string_view _defaultType;
        const bool _throwOnUnknownType;

      public:
        using Ptr = std::unique_ptr<ValueDeserializersMap>;

        explicit ValueDeserializersMap(std::string_view defaultType, bool throwOnUnknownType = true,
                                       Deserializers valueDeserializers = {});

        ValueDeserializersMap(const ValueDeserializersMap &) = delete;
        ValueDeserializersMap(ValueDeserializersMap &&) noexcept = default;

        ValueDeserializersMap &operator=(const ValueDeserializersMap &) = delete;
        ValueDeserializersMap &operator=(ValueDeserializersMap &&) = delete;

        [[nodiscard]] DeserializersMap &getDeserializersMap();

        void set(std::string_view type, IDeserializer::Ptr deserializer);

        [[nodiscard]] const IDeserializer &getDeserializerFor(std::optional<std::string_view> type) const override;

      private:
        [[nodiscard]] const IDeserializer &getDefaultDeserializer() const;

        [[nodiscard]] const IDeserializer *findDeserializerFor(std::string_view type) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ValueDeserializersMap.hpp"
#endif
