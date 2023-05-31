#pragma once

#include <memory>

#include "SevenBit/Conf/AppSettingsConfiguration.hpp"
#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"

namespace sb::cf
{
    INLINE AppSettingsConfigurationSource::AppSettingsConfigurationSource(std::string environmentName)
        : _envName(std::move(environmentName))
    {
    }

    INLINE AppSettingsConfigurationSource::SPtr AppSettingsConfigurationSource::create(std::string environmentName)
    {
        return AppSettingsConfigurationSource::SPtr(new AppSettingsConfigurationSource{std::move(environmentName)});
    }

    INLINE const std::string &AppSettingsConfigurationSource::getEnvName() const { return _envName; }

    INLINE IConfigurationProvider::Ptr AppSettingsConfigurationSource::build(IConfigurationBuilder &builder)
    {
        ChainedConfigurationSource::SPtr _chain =
            ChainedConfigurationSource::create({JsonFileConfigurationSource::create("appsettings.json", true)});
        if (!getEnvName().empty())
        {
            _chain->add(JsonFileConfigurationSource::create("appsettings." + getEnvName() + ".json", true));
        }
        return _chain->build(builder);
    }

} // namespace sb::cf