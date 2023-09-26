#pragma once

#include <memory>

#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf
{
    INLINE ChainedConfigurationSource::ChainedConfigurationSource(std::vector<IConfigurationSource::SPtr> sources)
        : _sources(std::move(sources))
    {
        if (!_sources.empty() &&
            std::any_of(_sources.begin(), _sources.end(), [](const auto &source) { return !source; }))
        {
            throwSourceNullException();
        }
    }

    INLINE ChainedConfigurationSource::Ptr ChainedConfigurationSource::create(
        std::vector<IConfigurationSource::SPtr> sources)
    {
        return std::make_unique<ChainedConfigurationSource>(std::move(sources));
    }

    INLINE void ChainedConfigurationSource::add(IConfigurationSource::SPtr source)
    {
        if (!source)
        {
            throwSourceNullException();
        }
        _sources.push_back(std::move(source));
    }

    INLINE void ChainedConfigurationSource::throwSourceNullException() const
    {
        throw NullPointerException{"Source cannot be null"};
    }

    INLINE IConfigurationProvider::Ptr ChainedConfigurationSource::build(IConfigurationBuilder &builder)
    {
        std::vector<IConfigurationProvider::Ptr> providers;
        providers.reserve(_sources.size());
        for (auto &source : _sources)
        {
            if (source)
            {
                providers.emplace_back(source->build(builder));
            }
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