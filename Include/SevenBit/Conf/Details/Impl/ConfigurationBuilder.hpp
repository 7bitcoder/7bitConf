#pragma once

#include <memory>

#include "SevenBit/Conf/ConfigurationBuilder.hpp"

namespace sb::cf
{
    INLINE ConfigurationBuilder::ConfigurationBuilder(std::vector<IConfigurationSource::SPtr> sources)
        : _sources(std::move(sources))
    {
    }

    INLINE IConfigurationBuilder &ConfigurationBuilder::add(IConfigurationSource::SPtr source)
    {
        _sources.push_back(std::move(source));
        return *this;
    }

    INLINE IConfigurationRoot::Ptr ConfigurationBuilder::build() { return std::make_unique<Configuration>(_sources); }

    INLINE bool ConfigurationBuilder::hasAnySources() const { return !_sources.empty(); }

    INLINE const std::vector<IConfigurationSource::SPtr> &ConfigurationBuilder::getSources() const { return _sources; }
} // namespace sb::cf