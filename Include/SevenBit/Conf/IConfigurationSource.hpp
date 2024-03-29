#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationProvider.hpp"

namespace sb::cf
{
    struct IConfigurationBuilder;

    struct IConfigurationSource
    {
        using Ptr = std::unique_ptr<IConfigurationSource>;
        using SPtr = std::shared_ptr<IConfigurationSource>;

        virtual IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) = 0;

        virtual ~IConfigurationSource() = default;
    };
} // namespace sb::cf
