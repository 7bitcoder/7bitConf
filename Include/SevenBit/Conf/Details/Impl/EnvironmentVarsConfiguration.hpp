#pragma once

#include <cstdlib>
#include <memory>

#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/EnvironmentVarsConfiguration.hpp"

#ifdef _WIN32
extern "C" __declspec(dllimport) char **_environ;
#define _7BIT_CONF_ENV_PTR _environ
#else
extern "C" char **environ;
#define _7BIT_CONF_ENV_PTR environ
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
            new EnvironmentVarsConfigurationSource{std::move(prefix), parserCfg});
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
        auto envVars = getEnvVars();
        set(_source->getSettingParser().parseAll(envVars.begin(), envVars.end()));
    }

    INLINE std::vector<std::string_view> EnvironmentVarsConfigurationProvider::getEnvVars()
    {
        std::vector<std::string_view> result;
        auto &prefix = _source->getPrefix();
        for (auto env = _7BIT_CONF_ENV_PTR; *env; env++)
        {
            std::string_view envStr = *env;
            if (prefix.empty())
            {
                result.push_back(envStr);
            }
            else if (details::utils::startsWith(envStr, prefix))
            {
                result.push_back(envStr.substr(prefix.size()));
            }
        }
        return result;
    }
} // namespace sb::cf