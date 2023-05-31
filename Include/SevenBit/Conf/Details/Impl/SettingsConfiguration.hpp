#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"
#include "SevenBit/Conf/SettingsConfiguration.hpp"

namespace sb::cf
{
    INLINE SettingsConfigurationSource::SettingsConfigurationSource(
        std::vector<std::pair<std::string_view, JsonValue>> settings, SettingParserConfig parserCfg)
        : _settings(std::move(settings)), _settingsParser(std::move(parserCfg))
    {
    }

    INLINE SettingsConfigurationSource::SPtr SettingsConfigurationSource::create(
        std::vector<std::pair<std::string_view, JsonValue>> settings, SettingParserConfig parserCfg)
    {
        return SettingsConfigurationSource::SPtr(
            new SettingsConfigurationSource{std::move(settings), std::move(parserCfg)});
    }

    INLINE const details::SettingParser &SettingsConfigurationSource::getSettingParser() const
    {
        return _settingsParser;
    }

    INLINE IConfigurationProvider::Ptr SettingsConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<SettingsConfigurationProvider>(shared_from_this());
    }

    INLINE SettingsConfigurationProvider::SettingsConfigurationProvider(SettingsConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
    }

    INLINE void SettingsConfigurationProvider::load()
    {
        clear();
        for (auto &[key, value] : *_source)
        {
            update(_source->getSettingParser().parseSetting(key, value));
        }
    }
} // namespace sb::cf