#pragma once

#include "SevenBit/Conf/CommandLineConfiguration.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf
{
    INLINE CommandLineConfigurationSource::CommandLineConfigurationSource(std::vector<std::string_view> args,
                                                                          ISettingParser::Ptr parser)
        : _args(std::move(args)), _parser(std::move(parser))
    {
        if (!_parser)
        {
            throw NullPointerException("Parser cannot be null");
        }
    }

    INLINE CommandLineConfigurationSource::SPtr CommandLineConfigurationSource::create(int argc,
                                                                                       const char *const *argv,
                                                                                       ISettingParser::Ptr parser)
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
        std::vector<std::string_view> args, ISettingParser::Ptr parser)
    {
        return CommandLineConfigurationSource::SPtr(
            new CommandLineConfigurationSource{std::move(args), std::move(parser)});
    }

    INLINE const std::vector<std::string_view> &CommandLineConfigurationSource::getArgs() const { return _args; }

    INLINE const ISettingParser &CommandLineConfigurationSource::getSettingParser() const { return *_parser; }

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
        auto &parser = _source->getSettingParser();
        for (auto &setting : _source->getArgs())
        {
            auto [keys, value] = parser.parse(setting);
            update(keys, std::move(value));
        }
    }
} // namespace sb::cf