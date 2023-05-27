#pragma once

#include <memory>

#include "SevenBit/Conf/AppSettingsConfiguration.hpp"
#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"

namespace sb::cf
{
    INLINE AppSettingsConfigurationSource::AppSettingsConfigurationSource(std::string envName)
        : _envName(std::move(envName))
    {
        add(std::make_unique<JsonFileConfigurationSource>("appsettings.json", true));
        if (!_envName.empty())
        {
            add(std::make_unique<JsonFileConfigurationSource>("appsettings." + _envName + ".json", true));
        }
    }

    INLINE const std::string &AppSettingsConfigurationSource::getEnvName() { return _envName; }

} // namespace sb::cf