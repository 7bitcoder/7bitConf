#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SevenBit/Conf/Details/OptionsParser.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"

namespace sb::cf
{
    EXPORT class CommandLineConfigurationSource : public IConfigurationSource,
                                                  public std::enable_shared_from_this<CommandLineConfigurationSource>
    {
      private:
        std::vector<std::string_view> _args;
        OptionsParser _parser;

        CommandLineConfigurationSource(std::vector<std::string_view> args, OptionsParserConfig config);

      public:
        using Ptr = std::unique_ptr<CommandLineConfigurationSource>;
        using SPtr = std::shared_ptr<CommandLineConfigurationSource>;

        static SPtr create(int argc, char **argv, OptionsParserConfig config = {});

        static SPtr create(std::vector<std::string_view> args, OptionsParserConfig config = {});

        const std::vector<std::string_view> &getArgs() const;

        const OptionsParser &getOptionsParser();

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