#pragma once

#include "SevenBit/Conf/Details/Require.hpp"
#include "SevenBit/Conf/Sources/CommandLineConfiguration.hpp"

namespace sb::cf
{
    INLINE CommandLineConfigurationSource::CommandLineConfigurationSource(std::vector<std::string_view> args,
                                                                          ISettingsParser::Ptr parser)
        : _args(std::move(args)), _parser(std::move(parser))
    {
        details::Require::notNull(_parser);
    }

    INLINE CommandLineConfigurationSource::SPtr CommandLineConfigurationSource::create(int argc,
                                                                                       const char *const *argv,
                                                                                       ISettingsParser::Ptr parser)
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
        return create(std::move(args), std::move(parser));
    }

    INLINE CommandLineConfigurationSource::SPtr CommandLineConfigurationSource::create(
        std::vector<std::string_view> args, ISettingsParser::Ptr parser)
    {
        return CommandLineConfigurationSource::SPtr(
            new CommandLineConfigurationSource{std::move(args), std::move(parser)});
    }

    INLINE const std::vector<std::string_view> &CommandLineConfigurationSource::getArgs() const { return _args; }

    INLINE const ISettingsParser &CommandLineConfigurationSource::getSettingsParser() const { return *_parser; }

    INLINE IConfigurationProvider::Ptr CommandLineConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<CommandLineConfigurationProvider>(shared_from_this());
    }

    INLINE CommandLineConfigurationProvider::CommandLineConfigurationProvider(
        CommandLineConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
        details::Require::notNull(_source);
    }

    INLINE void CommandLineConfigurationProvider::load()
    {
        clear();
        set(_source->getSettingsParser().parse(_source->getArgs()));
    }
} // namespace sb::cf
