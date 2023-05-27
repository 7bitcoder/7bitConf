#pragma once

#include <memory>

#include "SevenBit/Conf/CommandLineConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    INLINE CommandLineConfigurationProvider::CommandLineConfigurationProvider(CommandLineConfigurationSource source)
        : _source(std::move(source))
    {
    }

    INLINE void CommandLineConfigurationProvider::load() { _configuration.clear(); }

    INLINE const JsonObject &CommandLineConfigurationProvider::get() const { return _configuration; }

    INLINE CommandLineConfigurationSource::CommandLineConfigurationSource(int argc, char **argv)
        : _argc(argc), _argv(argv)
    {
    }

    INLINE int CommandLineConfigurationSource::getArgc() { return _argc; }

    INLINE char **CommandLineConfigurationSource::getArgv() { return _argv; }

    INLINE IConfigurationProvider::Ptr CommandLineConfigurationSource::build() const
    {
        return std::make_unique<CommandLineConfigurationProvider>(*this);
    }
} // namespace sb::cf