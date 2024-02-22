#pragma once

#include <memory>

#include "SevenBit/Conf/Details/Require.hpp"
#include "SevenBit/Conf/Sources/ChainedConfiguration.hpp"

namespace sb::cf
{
    INLINE ChainedConfigurationSource::ChainedConfigurationSource(std::vector<IConfigurationSource::SPtr> sources)
        : _sources(std::move(sources))
    {
        for (auto &source : _sources)
        {
            details::Require::notNull(source);
        }
    }

    INLINE ChainedConfigurationSource::Ptr ChainedConfigurationSource::create(
        std::vector<IConfigurationSource::SPtr> sources)
    {
        return std::make_unique<ChainedConfigurationSource>(std::move(sources));
    }

    INLINE void ChainedConfigurationSource::add(IConfigurationSource::SPtr source)
    {
        details::Require::notNull(source);
        _sources.push_back(std::move(source));
    }

    INLINE IConfigurationProvider::Ptr ChainedConfigurationSource::build(IConfigurationBuilder &builder)
    {
        std::vector<IConfigurationProvider::Ptr> providers;
        providers.reserve(_sources.size());
        for (auto &source : _sources)
        {
            details::Require::notNull(source);
            providers.emplace_back(source->build(builder));
        }
        return std::make_unique<ChainedConfigurationProvider>(std::move(providers));
    }

    INLINE ChainedConfigurationProvider::ChainedConfigurationProvider(
        std::vector<IConfigurationProvider::Ptr> providers)
        : _providers(std::move(providers))
    {
        for (auto &provider : _providers)
        {
            details::Require::notNull(provider);
        }
    }

    INLINE void ChainedConfigurationProvider::load()
    {
        clear();
        for (auto &provider : _providers)
        {
            details::Require::notNull(provider);
            provider->load();
            update(provider->getConfiguration());
        }
    }

} // namespace sb::cf
