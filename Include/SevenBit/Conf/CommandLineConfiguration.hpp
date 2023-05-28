#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class CommandLineConfigurationSource : public IConfigurationSource,
                                                  public std::enable_shared_from_this<CommandLineConfigurationSource>
    {
      private:
        int _argc;
        char **_argv;

        CommandLineConfigurationSource(int argc, char **argv);

      public:
        using Ptr = std::unique_ptr<CommandLineConfigurationSource>;
        using SPtr = std::shared_ptr<CommandLineConfigurationSource>;

        static SPtr create(int argc, char **argv);

        int getArgc();

        char **getArgv();

        IConfigurationProvider::Ptr build() override;
    };

    EXPORT class CommandLineConfigurationProvider : public ConfigurationProviderBase<CommandLineConfigurationSource>
    {
      public:
        using ConfigurationProviderBase<CommandLineConfigurationSource>::ConfigurationProviderBase;

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/CommandLineConfiguration.hpp"
#endif