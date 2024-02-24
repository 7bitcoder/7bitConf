#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    struct IConfigurationProvider
    {
        using Ptr = std::unique_ptr<IConfigurationProvider>;

        virtual void load() = 0;

        [[nodiscard]] virtual const JsonObject &getConfiguration() const = 0;

        virtual ~IConfigurationProvider() = default;
    };
} // namespace sb::cf
