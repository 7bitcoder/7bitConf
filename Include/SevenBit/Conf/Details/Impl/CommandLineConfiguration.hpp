#pragma once

#include <algorithm>
#include <memory>

#include "SevenBit/Conf/CommandLineConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    INLINE CommandLineConfigurationSource::CommandLineConfigurationSource(std::vector<std::string> args,
                                                                          OptionsParserConfig config)
        : _args(std::move(args)), _parser(std::move(config))
    {
    }

    INLINE CommandLineConfigurationSource::SPtr CommandLineConfigurationSource::create(int argc, char **argv,
                                                                                       OptionsParserConfig config)
    {
        std::vector<std::string> args;
        if (argc > 1)
        {
            args.reserve(argc - 1);
            for (auto i = 1; i < argc; ++i)
            {
                args.push_back(argv[i]);
            }
        }
        return create(std::move(args), std::move(config));
    }

    INLINE CommandLineConfigurationSource::SPtr CommandLineConfigurationSource::create(std::vector<std::string> args,
                                                                                       OptionsParserConfig config)
    {
        return CommandLineConfigurationSource::SPtr(
            new CommandLineConfigurationSource{std::move(args), std::move(config)});
    }

    INLINE const std::vector<std::string> &CommandLineConfigurationSource::getArgs() const { return _args; }

    INLINE const OptionsParser &CommandLineConfigurationSource::getOptionsParser() { return _parser; }

    INLINE IConfigurationProvider::Ptr CommandLineConfigurationSource::build()
    {
        return std::make_unique<CommandLineConfigurationProvider>(shared_from_this());
    }

    INLINE void CommandLineConfigurationProvider::load()
    {
        clear();
        for (auto &arg : _source->getArgs())
        {
            update(_source->getOptionsParser().parseOption(arg));
        }
    }
} // namespace sb::cf