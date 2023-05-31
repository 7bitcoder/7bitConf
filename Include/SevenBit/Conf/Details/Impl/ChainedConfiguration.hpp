#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    INLINE ChainedConfigurationSource::ChainedConfigurationSource(std::vector<IConfigurationSource::SPtr> sources)
        : _sources(std::move(sources))
    {
    }

    INLINE ChainedConfigurationSource::SPtr ChainedConfigurationSource::create(
        std::vector<IConfigurationSource::SPtr> sources)
    {
        return ChainedConfigurationSource::SPtr(new ChainedConfigurationSource{std::move(sources)});
    }

    INLINE void ChainedConfigurationSource::add(IConfigurationSource::SPtr source)
    {
        _sources.push_back(std::move(source));
    }

    INLINE IConfigurationProvider::Ptr ChainedConfigurationSource::build(IConfigurationBuilder &builder)
    {
        std::vector<IConfigurationProvider::Ptr> providers;
        providers.reserve(_sources.size());
        for (auto &source : _sources)
        {
            providers.emplace_back(source->build(builder));
        }
        return std::make_unique<ChainedConfigurationProvider>(std::move(providers));
    }

    INLINE ChainedConfigurationProvider::ChainedConfigurationProvider(
        std::vector<IConfigurationProvider::Ptr> providers)
        : _providers(std::move(providers))
    {
    }

    INLINE void ChainedConfigurationProvider::load()
    {
        clear();
        for (auto &provider : _providers)
        {
            provider->load();
            update(std::move(provider->getConfiguration()));
        }
    }
} // namespace sb::cf