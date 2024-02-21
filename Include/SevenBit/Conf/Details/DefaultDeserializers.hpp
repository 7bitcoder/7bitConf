#pragma once

#include "SevenBit/Conf/Details/Deserializers.hpp"

namespace sb::cf
{
    struct DefaultDeserializers
    {
        static void add(std::vector<std::pair<std::string_view, IDeserializer::Ptr>> &deserializers)
        {
            deserializers.emplace_back("string", std::make_unique<details::StringDeserializer>());
            deserializers.emplace_back("bool", std::make_unique<details::BoolDeserializer>());
            deserializers.emplace_back("int", std::make_unique<details::IntDeserializer>());
            deserializers.emplace_back("double", std::make_unique<details::DoubleDeserializer>());
            deserializers.emplace_back("uint", std::make_unique<details::UIntDeserializer>());
            deserializers.emplace_back("json", std::make_unique<details::JsonDeserializer>());
            deserializers.emplace_back("null", std::make_unique<details::NullDeserializer>());
        }
    };
} // namespace sb::cf
