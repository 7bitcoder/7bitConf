#pragma once

#include <cstdlib>

#include "SevenBit/Conf/Details/Require.hpp"
#include "SevenBit/Conf/Details/StringUtils.hpp"
#include "SevenBit/Conf/Sources/EnvironmentVarsConfiguration.hpp"

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
                                                                                  ISettingsParser::Ptr parser)
        : _prefix(std::move(prefix)), _parser(std::move(parser))
    {
        details::Require::notNull(_parser);
    }

    INLINE EnvironmentVarsConfigurationSource::SPtr EnvironmentVarsConfigurationSource::create(
        std::string prefix, ISettingsParser::Ptr parser)
    {
        return EnvironmentVarsConfigurationSource::SPtr(
            new EnvironmentVarsConfigurationSource{std::move(prefix), std::move(parser)});
    }

    INLINE const std::string &EnvironmentVarsConfigurationSource::getPrefix() const { return _prefix; }

    INLINE const ISettingsParser &EnvironmentVarsConfigurationSource::getSettingParser() const { return *_parser; }

    INLINE IConfigurationProvider::Ptr EnvironmentVarsConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<EnvironmentVarsConfigurationProvider>(shared_from_this());
    }

    INLINE EnvironmentVarsConfigurationProvider::EnvironmentVarsConfigurationProvider(
        EnvironmentVarsConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
        details::Require::notNull(_source);
    }

    INLINE void EnvironmentVarsConfigurationProvider::load() { set(_source->getSettingParser().parse(getEnvVars())); }

    INLINE std::vector<std::string_view> EnvironmentVarsConfigurationProvider::getEnvVars() const
    {
        std::vector<std::string_view> result;
        auto &prefix = _source->getPrefix();
        for (auto env = _7BIT_CONF_ENV_PTR; *env; env++)
        {
            if (std::string_view envStr = *env; details::StringUtils::startsWith(envStr, prefix))
            {
                result.push_back(envStr.substr(prefix.size()));
            }
        }
        return result;
    }
} // namespace sb::cf
