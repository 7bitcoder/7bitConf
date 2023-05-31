#pragma once

#include <memory>
#include <string>

#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class AppSettingsConfigurationSource : public IConfigurationSource
    {
      private:
        std::string _envName;

        AppSettingsConfigurationSource(std::string environmentName);

      public:
        using Ptr = std::unique_ptr<AppSettingsConfigurationSource>;
        using SPtr = std::shared_ptr<AppSettingsConfigurationSource>;

        static SPtr create(std::string environmentName = "");

        const std::string &getEnvName() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/AppSettingsConfiguration.hpp"
#endif