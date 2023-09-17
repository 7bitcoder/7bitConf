#pragma once

#include "SevenBit/Conf/CommandLineConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    INLINE CommandLineConfigurationSource::CommandLineConfigurationSource(std::vector<std::string_view> args,
                                                                          SettingParserConfig config)
        : _args(std::move(args)), _parser(config)
    {
    }

    INLINE CommandLineConfigurationSource::SPtr CommandLineConfigurationSource::create(int argc,
                                                                                       const char *const *argv,
                                                                                       SettingParserConfig config)
    {
        std::vector<std::string_view> args;
        if (argc > 1)
        {
            args.reserve(argc - 1);
            for (size_t i = 1; i < argc; ++i)
            {
                args.emplace_back(argv[i]);
            }
        }
        return create(std::move(args), config);
    }

    INLINE CommandLineConfigurationSource::SPtr CommandLineConfigurationSource::create(
        std::vector<std::string_view> args, SettingParserConfig config)
    {
        return CommandLineConfigurationSource::SPtr(new CommandLineConfigurationSource{std::move(args), config});
    }

    INLINE const std::vector<std::string_view> &CommandLineConfigurationSource::getArgs() const { return _args; }

    INLINE const details::SettingParser &CommandLineConfigurationSource::getOptionsParser() const { return _parser; }

    INLINE IConfigurationProvider::Ptr CommandLineConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<CommandLineConfigurationProvider>(shared_from_this());
    }

    INLINE CommandLineConfigurationProvider::CommandLineConfigurationProvider(
        CommandLineConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
    }

    INLINE void CommandLineConfigurationProvider::load()
    {
        clear();
        set(_source->getOptionsParser().parseAll(_source->getArgs().begin(), _source->getArgs().end()));
    }
} // namespace sb::cf