#pragma once

#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/InMemoryConfiguration.hpp"

namespace sb::cf
{
    INLINE InMemoryConfigurationSource::InMemoryConfigurationSource(
        std::vector<std::pair<std::string_view, JsonValue>> settings)
        : _settings(std::move(settings))
    {
    }

    INLINE InMemoryConfigurationSource::SPtr InMemoryConfigurationSource::create(
        std::vector<std::pair<std::string_view, JsonValue>> settings)
    {
        return InMemoryConfigurationSource::SPtr(new InMemoryConfigurationSource{std::move(settings)});
    }

    INLINE InMemoryConfigurationProvider::Ptr InMemoryConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<InMemoryConfigurationProvider>(shared_from_this());
    }

    INLINE InMemoryConfigurationProvider::InMemoryConfigurationProvider(InMemoryConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
        details::utils::assertPtr(_source);
    }

    INLINE void InMemoryConfigurationProvider::load()
    {
        clear();
        for (auto &[key, value] : *_source)
        {
            JsonObject result{};
            details::JsonExt::deepGetOrOverride(result, key) = std::move(value);
            update(std::move(result));
        }
    }
} // namespace sb::cf
