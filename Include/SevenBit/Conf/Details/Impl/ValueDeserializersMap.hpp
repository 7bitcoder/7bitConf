#pragma once

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    INLINE std::map<std::string, IDeserializer::Ptr, ValueDeserializersMap::CaseInsensitiveLess> &
    ValueDeserializersMap::getDeserializersMap()
    {
        return _deserializersLookup;
    }

    INLINE void ValueDeserializersMap::set(std::string_view type, IDeserializer::Ptr deserializer)
    {
        _deserializersLookup[std::string{type}] = std::move(deserializer);
    }

    INLINE const IDeserializer *ValueDeserializersMap::getDeserializerFor(std::string_view typeStr) const
    {
        auto it = _deserializersLookup.find(typeStr);
        return it != _deserializersLookup.end() ? it->second.get() : nullptr;
    }
} // namespace sb::cf::details
