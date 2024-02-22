#pragma once

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Deserializers.hpp"
#include "SevenBit/Conf/Details/ValueDeserializersMap.hpp"

namespace sb::cf::details
{
    struct DefaultDeserializers
    {
        static void add(std::vector<std::pair<std::string_view, IDeserializer::Ptr>> &deserializers);

        static void add(ValueDeserializersMap &deserializersMap);
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/DefaultDeserializers.hpp"
#endif
