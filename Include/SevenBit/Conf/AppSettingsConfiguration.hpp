#pragma once

#include <string>

#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class AppSettingsConfigurationSource : public IConfigurationSource,
                                                  public std::enable_shared_from_this<AppSettingsConfigurationSource>
    {
      private:
        std::string _envName;

        AppSettingsConfigurationSource(std::string envName);

      public:
        using Ptr = std::unique_ptr<AppSettingsConfigurationSource>;
        using SPtr = std::shared_ptr<AppSettingsConfigurationSource>;

        static SPtr create(std::string envName = "");

        const std::string &getEnvName();

        IConfigurationProvider::Ptr build() override;
    };

    EXPORT class AppSettingsConfigurationProvider : public ConfigurationProviderBase<AppSettingsConfigurationSource>
    {
      public:
        using ConfigurationProviderBase<AppSettingsConfigurationSource>::ConfigurationProviderBase;

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/AppSettingsConfiguration.hpp"
#endif