#pragma once

#include <memory>
#include <string>

#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    class EXPORT AppSettingsConfigurationSource : public IConfigurationSource
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

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/AppSettingsConfiguration.hpp"
#endif