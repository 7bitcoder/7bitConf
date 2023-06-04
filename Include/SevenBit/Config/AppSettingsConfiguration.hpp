#pragma once

#include <memory>
#include <string>

#include "SevenBit/Config/ChainedConfiguration.hpp"
#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class AppSettingsConfigurationSource : public IConfigurationSource
    {
      private:
        std::string _envName;

      public:
        using Ptr = std::unique_ptr<AppSettingsConfigurationSource>;
        using SPtr = std::shared_ptr<AppSettingsConfigurationSource>;

        AppSettingsConfigurationSource(std::string environmentName);

        static Ptr create(std::string environmentName = "");

        const std::string &getEnvironmentName() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/AppSettingsConfiguration.hpp"
#endif