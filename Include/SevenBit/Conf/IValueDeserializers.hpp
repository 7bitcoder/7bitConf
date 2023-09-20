#pragma once

#include <memory>
#include <string_view>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IDeserializer.hpp"

namespace sb::cf
{
    struct IValueDeserializers
    {
        using Ptr = std::unique_ptr<IValueDeserializers>;

        virtual const IDeserializer *getDeserializerFor(std::string_view type) const = 0;

        virtual const IDeserializer &getDefaultDeserializer() const = 0;

        virtual ~IValueDeserializers() = default;
    };
} // namespace sb::cf