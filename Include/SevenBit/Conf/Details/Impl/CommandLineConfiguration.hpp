#pragma once

#include <memory>

#include "SevenBit/Conf/CommandLineConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    INLINE CommandLineConfigurationSource::CommandLineConfigurationSource(int argc, char **argv)
        : _argc(argc), _argv(argv)
    {
    }

    INLINE CommandLineConfigurationSource::SPtr CommandLineConfigurationSource::create(int argc, char **argv)
    {
        return CommandLineConfigurationSource::SPtr(new CommandLineConfigurationSource{argc, argv});
    }

    INLINE int CommandLineConfigurationSource::getArgc() { return _argc; }

    INLINE char **CommandLineConfigurationSource::getArgv() { return _argv; }

    INLINE IConfigurationProvider::Ptr CommandLineConfigurationSource::build()
    {
        return std::make_unique<CommandLineConfigurationProvider>(shared_from_this());
    }

    INLINE void CommandLineConfigurationProvider::load() { clear(); }
} // namespace sb::cf