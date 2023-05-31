#pragma once

#include <memory>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/EnvironmentVarsConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

extern char **environ;

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