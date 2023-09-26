#pragma once

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    INLINE std::vector<std::pair<std::string, IDeserializer::Ptr>> &ValueDeserializersMap::getDeserializersLookup()
    {
        return _deserializersLookup;
    }

    INLINE void ValueDeserializersMap::add(std::string_view type, IDeserializer::Ptr deserializer)
    {
        if (!deserializer)
        {
            throw NullPointerException("Deserializer cannot be null");
        }
        _deserializersLookup.emplace_back(std::string{type}, std::move(deserializer));
    }

    INLINE const IDeserializer *ValueDeserializersMap::getDeserializerFor(std::string_view typeStr) const
    {
        for (auto &[type, deserializer] : _deserializersLookup)
        {
            if (utils::ignoreCaseEqual(type, typeStr))
            {
                return deserializer.get();
            }
        }
        return nullptr;
    }
} // namespace sb::cf::details
