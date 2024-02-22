#pragma once

#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    INLINE ValueDeserializersMap::ValueDeserializersMap(const std::string_view defaultType,
                                                        const bool throwOnUnknownType, Deserializers valueDeserializers)
        : _defaultType(defaultType), _throwOnUnknownType(throwOnUnknownType)
    {
        for (auto &[type, deserializer] : valueDeserializers)
        {
            set(type, std::move(deserializer));
        }
    }

    INLINE void ValueDeserializersMap::set(const std::string_view type, IDeserializer::Ptr deserializer)
    {
        _deserializersMap[std::string{type}] = std::move(deserializer);
    }

    INLINE const IDeserializer &ValueDeserializersMap::getDeserializerFor(
        const std::optional<std::string_view> type) const
    {
        if (const auto deserializer = type ? findDeserializerFor(*type) : &getDefaultDeserializer())
        {
            return *deserializer;
        }
        if (getThrowOnUnknownType())
        {
            throw ConfigException("Unknown type: '" + std::string{*type} + "' deserializer for type was not found");
        }
        return getDefaultDeserializer();
    }

    INLINE const ValueDeserializersMap::DeserializersMap &ValueDeserializersMap::getDeserializersMap() const
    {
        return _deserializersMap;
    }

    INLINE std::string_view ValueDeserializersMap::getDefaultType() const { return _defaultType; }

    INLINE bool ValueDeserializersMap::getThrowOnUnknownType() const { return _throwOnUnknownType; }

    INLINE const IDeserializer &ValueDeserializersMap::getDefaultDeserializer() const
    {
        if (const auto deserializer = findDeserializerFor(getDefaultType()))
        {
            return *deserializer;
        }
        throw ConfigException("Unknown default type: '" + std::string{getDefaultType()} +
                              "' deserializer for type was not found");
    }

    INLINE const IDeserializer *ValueDeserializersMap::findDeserializerFor(const std::string_view type) const
    {
        const auto it = _deserializersMap.find(type);
        return it != _deserializersMap.end() ? it->second.get() : nullptr;
    }
} // namespace sb::cf::details
