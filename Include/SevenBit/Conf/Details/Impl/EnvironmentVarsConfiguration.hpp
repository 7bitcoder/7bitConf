#pragma once

#include <memory>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/EnvironmentVarsConfiguration.hpp"

extern char **environ;

namespace sb::cf
{
    INLINE EnvironmentVarsConfigurationProvider::EnvironmentVarsConfigurationProvider(
        EnvironmentVarsConfigurationSource source)
        : _source(std::move(source))
    {
    }

    INLINE void EnvironmentVarsConfigurationProvider::load()
    {
        _configuration.clear();
        for (auto &env : getEnvVars())
        {
            auto config = _source.getOptionsParser().parseOption(env);
            JsonObjectExt::deepMerge(_configuration, config);
        }
    }

    INLINE const JsonObject &EnvironmentVarsConfigurationProvider::get() const { return _configuration; }

    INLINE std::vector<std::string_view> EnvironmentVarsConfigurationProvider::getEnvVars()
    {
        std::vector<std::string_view> result;
        for (auto env = environ; *env; env++)
        {
            std::string_view envStr = *env;
            if (utils::startsWith(envStr, _source.getPrefix()))
            {
                result.push_back(envStr.substr(_source.getPrefix().size()));
            }
        }
        return result;
    }

    INLINE EnvironmentVarsConfigurationSource::EnvironmentVarsConfigurationSource(std::string prefix,
                                                                                  OptionsParserCfg parserCfg)
        : _prefix(std::move(prefix)), _parser(parserCfg)
    {
    }

    INLINE const std::string &EnvironmentVarsConfigurationSource::getPrefix() { return _prefix; }

    INLINE const OptionsParser &EnvironmentVarsConfigurationSource::getOptionsParser() { return _parser; }

    INLINE IConfigurationProvider::Ptr EnvironmentVarsConfigurationSource::build() const
    {
        return std::make_unique<EnvironmentVarsConfigurationProvider>(*this);
    }
} // namespace sb::cf