#pragma once

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Deserializers.hpp"

namespace sb::cf::details
{
    struct DefaultDeserializers
    {
        static void add(std::vector<std::pair<std::string_view, IDeserializer::Ptr>> &deserializers);
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/DefaultDeserializers.hpp"
#endif
