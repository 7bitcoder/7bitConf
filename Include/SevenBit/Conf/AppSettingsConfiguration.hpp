#pragma once

#include <string>

#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class AppSettingsConfigurationSource final : public ChainedConfigurationSource
    {
      private:
        std::string _envName;

      public:
        AppSettingsConfigurationSource(std::string envName = "");

        const std::string &getEnvName();
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/AppSettingsConfiguration.hpp"
#endif