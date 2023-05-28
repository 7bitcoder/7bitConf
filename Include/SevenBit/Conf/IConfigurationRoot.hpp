#pragma once

#include <memory>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfiguration.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    struct IConfigurationRoot : public IConfiguration
    {
        using Ptr = std::unique_ptr<IConfigurationRoot>;

        virtual void reload() = 0;

        virtual const std::vector<IConfigurationSource::SPtr> &getSources() const = 0;

        virtual ~IConfigurationRoot() = default;
    };
} // namespace sb::cf