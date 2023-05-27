#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    INLINE ChainedConfigurationSource::ChainedConfigurationSource(std::vector<IConfigurationSource::SPtr> sources)
        : _sources(std::move(sources))
    {
    }

    INLINE void ChainedConfigurationSource::add(IConfigurationSource::Ptr source)
    {
        _sources.push_back(std::move(source));
    }

    INLINE IConfigurationProvider::Ptr ChainedConfigurationSource::build() const
    {
        return std::make_unique<ChainedConfigurationProvider>(*this);
    }

    INLINE ChainedConfigurationProvider::ChainedConfigurationProvider(ChainedConfigurationSource source)
        : _source(std::move(source))
    {
    }

    INLINE void ChainedConfigurationProvider::load()
    {
        _configuration.clear();
        for (auto &source : _source)
        {
            auto provider = source->build();
            provider->load();
            JsonObjectExt::deepMerge(_configuration, provider->get());
        }
    }

    INLINE const JsonObject &ChainedConfigurationProvider::get() const { return _configuration; }
} // namespace sb::cf