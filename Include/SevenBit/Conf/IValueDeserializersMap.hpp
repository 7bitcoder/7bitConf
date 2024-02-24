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

        [[nodiscard]] virtual const IDeserializer &getDeserializerFor(std::optional<std::string_view> type) const = 0;

        virtual ~IValueDeserializersMap() = default;
    };
} // namespace sb::cf
