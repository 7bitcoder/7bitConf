#pragma once

#include <memory>
#include <string_view>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IDeserializer.hpp"

namespace sb::cf
{
    struct IValueDeserializersMap
    {
        using Ptr = std::unique_ptr<IValueDeserializersMap>;

        virtual const IDeserializer *getDeserializerFor(std::string_view type) const = 0;

        virtual ~IValueDeserializersMap() = default;
    };
} // namespace sb::cf