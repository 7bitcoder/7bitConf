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
                                                                                  ISettingParser::Ptr parser)
        : _prefix(std::move(prefix)), _parser(std::move(parser))
    {
        if (!_parser)
        {
            throw std::invalid_argument("parser cannot be null");
        }
    }

    INLINE EnvironmentVarsConfigurationSource::SPtr EnvironmentVarsConfigurationSource::create(
        std::string prefix, ISettingParser::Ptr parser)
    {
        return EnvironmentVarsConfigurationSource::SPtr(
            new EnvironmentVarsConfigurationSource{std::move(prefix), std::move(parser)});
    }

    INLINE const std::string &EnvironmentVarsConfigurationSource::getPrefix() { return _prefix; }

    INLINE const ISettingParser &EnvironmentVarsConfigurationSource::getSettingParser() { return *_parser; }

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
        auto &parser = _source->getSettingParser();
        for (auto &setting : getEnvVars())
        {
            auto [keys, value] = parser.parse(setting);
            update(keys, value);
        }
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