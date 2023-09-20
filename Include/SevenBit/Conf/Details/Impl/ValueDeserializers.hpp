#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Details/ValueDeserializers.hpp"

namespace sb::cf::details
{
    INLINE std::vector<std::pair<std::string, std::unique_ptr<IDeserializer>>> &ValueDeserializers::
        getDeserializersLookup()
    {
        return _deserializersLookup;
    }

    INLINE void ValueDeserializers::add(std::string_view type, std::unique_ptr<IDeserializer> deserializer)
    {
        if (!deserializer)
        {
            throw std::invalid_argument("Deserializer cannot be null");
        }
        _deserializersLookup.emplace_back(std::string{type}, std::move(deserializer));
    }

    INLINE const IDeserializer *ValueDeserializers::getDeserializerFor(std::string_view typeStr) const
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

    INLINE const IDeserializer &ValueDeserializers::getDefaultDeserializer() const
    {
        if (_deserializersLookup.empty())
        {
            throw std::runtime_error("No default deserializer registered");
        }
        return *_deserializersLookup.front().second;
    }
} // namespace sb::cf::details
