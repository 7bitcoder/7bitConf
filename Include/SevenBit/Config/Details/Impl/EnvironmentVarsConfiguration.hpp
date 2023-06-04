#pragma once

#include <cstdlib>
#include <memory>

#include "SevenBit/Config/Details/JsonObjectExt.hpp"
#include "SevenBit/Config/EnvironmentVarsConfiguration.hpp"

#ifdef _WIN32
extern char **_environ;
#define _7BIT_CONFIG_ENV_PTR _environ
#else
extern char **environ;
#define _7BIT_CONFIG_ENV_PTR environ
#endif

namespace sb::cf
{
    INLINE EnvironmentVarsConfigurationSource::EnvironmentVarsConfigurationSource(std::string prefix,
                                                                                  SettingParserConfig parserConfig)
        : _prefix(std::move(prefix)), _parser(parserConfig)
    {
    }

    INLINE EnvironmentVarsConfigurationSource::SPtr EnvironmentVarsConfigurationSource::create(
        std::string prefix, SettingParserConfig parserCfg)
    {
        return EnvironmentVarsConfigurationSource::SPtr(
            new EnvironmentVarsConfigurationSource{std::move(prefix), std::move(parserCfg)});
    }

    INLINE const std::string &EnvironmentVarsConfigurationSource::getPrefix() { return _prefix; }

    INLINE const details::SettingParser &EnvironmentVarsConfigurationSource::getSettingParser() { return _parser; }

    INLINE IConfigurationProvider::Ptr EnvironmentVarsConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<EnvironmentVarsConfigurationProvider>(shared_from_this());
    }

    INLINE EnvironmentVarsConfigurationProvider::EnvironmentVarsConfigurationProvider(
        EnvironmentVarsConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
    }

    INLINE void EnvironmentVarsConfigurationProvider::load()
    {
        clear();
        for (auto &env : getEnvVars())
        {
            update(_source->getSettingParser().parseSetting(env));
        }
    }

    INLINE std::vector<std::string_view> EnvironmentVarsConfigurationProvider::getEnvVars()
    {
        std::vector<std::string_view> result;
        for (auto env = _7BIT_CONFIG_ENV_PTR; *env; env++)
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