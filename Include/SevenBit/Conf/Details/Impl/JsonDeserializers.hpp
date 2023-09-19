#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/Details/SettingDeserializers.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"

namespace sb::cf::details
{
    JsonDeserializers::Ptr JsonDeserializers::createDefault()
    {
        Ptr deserializers;
        deserializers->add("string", std::make_unique<StringDeserializer>());
        deserializers->add("bool", std::make_unique<BoolDeserializer>());
        deserializers->add("int", std::make_unique<IntDeserializer>());
        deserializers->add("double", std::make_unique<DoubleDeserializer>());
        deserializers->add("json", std::make_unique<JsonDeserializer>());
        deserializers->add("uint", std::make_unique<UIntDeserializer>());
        deserializers->add("null", std::make_unique<NullDeserializer>());
        return deserializers;
    }

    JsonDeserializers::IgnoreCaseUnorderedMap &JsonDeserializers::getDeserializersMap() { return _deserializersMap; }

    void JsonDeserializers::add(std::string_view type, std::unique_ptr<IJsonDeserializer> deserializer)
    {
        _deserializersMap[std::string{type}] = std::move(deserializer);
    }

    INLINE const IJsonDeserializer *JsonDeserializers::getDeserializer(std::string_view type) const
    {
        if (auto it = _deserializersMap.find(type); it != _deserializersMap.end())
        {
            return it->second.get();
        }
        return nullptr;
    }
} // namespace sb::cf::details