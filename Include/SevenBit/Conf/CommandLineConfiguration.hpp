#pragma once

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class CommandLineConfigurationSource : public IConfigurationSource
    {
      private:
        int _argc;
        char **_argv;

      public:
        CommandLineConfigurationSource(int argc, char **argv);

        int getArgc();

        char **getArgv();

        IConfigurationProvider::Ptr build() const override;
    };

    EXPORT class CommandLineConfigurationProvider final : public IConfigurationProvider
    {
      private:
        CommandLineConfigurationSource _source;

        JsonObject _configuration;

      public:
        CommandLineConfigurationProvider(CommandLineConfigurationSource source);

        void load() override;

        const JsonObject &get() const override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/CommandLineConfiguration.hpp"
#endif