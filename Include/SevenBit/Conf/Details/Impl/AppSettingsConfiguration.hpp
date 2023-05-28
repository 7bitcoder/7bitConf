#pragma once

#include <memory>

#include "SevenBit/Conf/AppSettingsConfiguration.hpp"
#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"

namespace sb::cf
{
    INLINE AppSettingsConfigurationSource::AppSettingsConfigurationSource(std::string envName)
        : _envName(std::move(envName))
    {
    }

    INLINE AppSettingsConfigurationSource::SPtr AppSettingsConfigurationSource::create(std::string envName)
    {
        return AppSettingsConfigurationSource::SPtr(new AppSettingsConfigurationSource{std::move(envName)});
    }

    INLINE const std::string &AppSettingsConfigurationSource::getEnvName() { return _envName; }

    INLINE IConfigurationProvider::Ptr AppSettingsConfigurationSource::build()
    {
        return std::make_unique<AppSettingsConfigurationProvider>(shared_from_this());
    }

    INLINE void AppSettingsConfigurationProvider::load()
    {
        clear();
        auto sources =
            ChainedConfigurationSource::create({JsonFileConfigurationSource::create("appsettings.json", true)});
        if (!_source->getEnvName().empty())
        {
            sources->add(JsonFileConfigurationSource::create("appsettings." + _source->getEnvName() + ".json", true));
        }
        setFrom(sources);
    }

} // namespace sb::cf