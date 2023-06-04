#pragma once

#include <memory>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/Json.hpp"

namespace sb::cf
{
    struct IConfigurationProvider
    {
        using Ptr = std::unique_ptr<IConfigurationProvider>;

        virtual void load() = 0;

        virtual JsonObject &getConfiguration() = 0;

        virtual const JsonObject &getConfiguration() const = 0;

        virtual ~IConfigurationProvider() = default;
    };
} // namespace sb::cf