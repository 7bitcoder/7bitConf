#pragma once

#include <memory>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/EnvironmentVarsConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

extern char **environ;

namespace sb::cf
{
    INLINE EnvironmentVarsConfigurationSource::EnvironmentVarsConfigurationSource(std::string prefix,
                                                                                  OptionsParserConfig parserConfig)
        : _prefix(std::move(prefix)), _parser(parserConfig)
    {
    }

    INLINE EnvironmentVarsConfigurationSource::SPtr EnvironmentVarsConfigurationSource::create(
        std::string prefix, OptionsParserConfig parserCfg)
    {
        return EnvironmentVarsConfigurationSource::SPtr(
            new EnvironmentVarsConfigurationSource{std::move(prefix), std::move(parserCfg)});
    }

    INLINE const std::string &EnvironmentVarsConfigurationSource::getPrefix() { return _prefix; }

    INLINE const OptionsParser &EnvironmentVarsConfigurationSource::getOptionsParser() { return _parser; }

    INLINE IConfigurationProvider::Ptr EnvironmentVarsConfigurationSource::build()
    {
        return std::make_unique<EnvironmentVarsConfigurationProvider>(shared_from_this());
    }

    INLINE void EnvironmentVarsConfigurationProvider::load()
    {
        clear();
        for (auto &env : getEnvVars())
        {
            update(_source->getOptionsParser().parseOption(env));
        }
    }

    INLINE std::vector<std::string_view> EnvironmentVarsConfigurationProvider::getEnvVars()
    {
        std::vector<std::string_view> result;
        for (auto env = environ; *env; env++)
        {
            std::string_view envStr = *env;
            if (utils::startsWith(envStr, _source->getPrefix()))
            {
                result.push_back(envStr.substr(_source->getPrefix().size()));
            }
        }
        return result;
    }
} // namespace sb::cf