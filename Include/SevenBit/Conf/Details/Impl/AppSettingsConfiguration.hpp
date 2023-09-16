#pragma once

#include "SevenBit/Conf/AppSettingsConfiguration.hpp"
#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"

namespace sb::cf
{
    INLINE AppSettingsConfigurationSource::AppSettingsConfigurationSource(std::string environmentName)
        : _envName(std::move(environmentName))
    {
    }

    INLINE AppSettingsConfigurationSource::Ptr AppSettingsConfigurationSource::create(std::string environmentName)
    {
        return AppSettingsConfigurationSource::Ptr(new AppSettingsConfigurationSource{std::move(environmentName)});
    }

    INLINE const std::string &AppSettingsConfigurationSource::getEnvironmentName() const { return _envName; }

    INLINE IConfigurationProvider::Ptr AppSettingsConfigurationSource::build(IConfigurationBuilder &builder)
    {
        ChainedConfigurationSource chain({JsonFileConfigurationSource::create("appsettings.json", true)});
        if (!getEnvironmentName().empty())
        {
            chain.add(JsonFileConfigurationSource::create("appsettings." + getEnvironmentName() + ".json", true));
        }
        return chain.build(builder);
    }

} // namespace sb::cf