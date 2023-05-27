#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Configuration.hpp"
#include "SevenBit/Conf/IConfiguration.hpp"
#include "SevenBit/Conf/IConfigurationBuilder.hpp"
#include "SevenBit/Conf/IConfigurationProvider.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    EXPORT class ConfigurationBuilder : public IConfigurationBuilder
    {
      private:
        std::vector<IConfigurationSource::SPtr> _sources;

      public:
        using Ptr = std::unique_ptr<ConfigurationBuilder>;

        ConfigurationBuilder(std::vector<IConfigurationSource::SPtr> = {});

        ConfigurationBuilder(ConfigurationBuilder &&) = default;
        ConfigurationBuilder(const ConfigurationBuilder &) = default;

        ConfigurationBuilder &operator=(ConfigurationBuilder &&) = default;
        ConfigurationBuilder &operator=(const ConfigurationBuilder &) = default;

        IConfigurationBuilder &add(IConfigurationSource::Ptr source);

        IConfigurationRoot::Ptr build();

        bool hasAnySources() const;

        const std::vector<IConfigurationSource::SPtr> &getSources() const;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ConfigurationBuilder.hpp"
#endif