#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{
    INLINE std::vector<std::pair<std::string, std::unique_ptr<IDeserializer>>> &ValueDeserializersMap::
        getDeserializersLookup()
    {
        return _deserializersLookup;
    }

    INLINE void ValueDeserializersMap::add(std::string_view type, std::unique_ptr<IDeserializer> deserializer)
    {
        if (!deserializer)
        {
            throw NullPointnerException("Deserializer cannot be null");
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
