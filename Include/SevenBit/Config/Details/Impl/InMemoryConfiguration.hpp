#pragma once

#include "SevenBit/Config/Details/JsonObjectExt.hpp"
#include "SevenBit/Config/InMemoryConfiguration.hpp"
#include "SevenBit/Config/Json.hpp"

namespace sb::cf
{
    INLINE InMemoryConfigurationSource::InMemoryConfigurationSource(
        std::vector<std::pair<std::string_view, JsonValue>> settings, SettingParserConfig parserCfg)
        : _settings(std::move(settings)), _settingsParser(std::move(parserCfg))
    {
    }

    INLINE InMemoryConfigurationSource::SPtr InMemoryConfigurationSource::create(
        std::vector<std::pair<std::string_view, JsonValue>> settings, SettingParserConfig parserCfg)
    {
        return InMemoryConfigurationSource::SPtr(
            new InMemoryConfigurationSource{std::move(settings), std::move(parserCfg)});
    }

    INLINE const details::SettingParser &InMemoryConfigurationSource::getSettingParser() const
    {
        return _settingsParser;
    }

    INLINE InMemoryConfigurationProvider::Ptr InMemoryConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<InMemoryConfigurationProvider>(shared_from_this());
    }

    INLINE InMemoryConfigurationProvider::InMemoryConfigurationProvider(InMemoryConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
    }

    INLINE void InMemoryConfigurationProvider::load()
    {
        clear();
        for (auto &[key, value] : *_source)
        {
            JsonObject result{};
            details::JsonObjectExt::getOrCreateInner(result, key) = std::move(value);
            update(std::move(result));
        }
    }
} // namespace sb::cf