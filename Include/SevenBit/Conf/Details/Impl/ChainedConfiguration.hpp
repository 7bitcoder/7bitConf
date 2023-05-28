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

    INLINE IConfigurationProvider::Ptr ChainedConfigurationSource::build()
    {
        return std::make_unique<ChainedConfigurationProvider>(shared_from_this());
    }

    INLINE void ChainedConfigurationProvider::load()
    {
        clear();
        for (auto &source : *_source)
        {
            updateFrom(source);
        }
    }
} // namespace sb::cf