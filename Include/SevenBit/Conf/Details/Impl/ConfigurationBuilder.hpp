#pragma once

#include "SevenBit/Conf/Configuration.hpp"
#include "SevenBit/Conf/ConfigurationBuilder.hpp"
#include "SevenBit/Conf/IConfigurationProvider.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

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

    INLINE IConfiguration::Ptr ConfigurationBuilder::build()
    {
        std::vector<IConfigurationProvider::Ptr> providers;
        providers.reserve(_sources.size());
        for (auto &source : _sources)
        {
            providers.emplace_back(source->build(*this));
        }
        return std::make_unique<Configuration>(std::move(providers));
    }

    INLINE void ConfigurationBuilder::clear() { _sources.clear(); }

    INLINE const std::vector<IConfigurationSource::SPtr> &ConfigurationBuilder::getSources() const { return _sources; }

    INLINE std::vector<IConfigurationSource::SPtr> &ConfigurationBuilder::getSources() { return _sources; }

    INLINE std::unordered_map<std::string, IObject::SPtr> &ConfigurationBuilder::getProperties() { return _properties; }

    INLINE const std::unordered_map<std::string, IObject::SPtr> &ConfigurationBuilder::getProperties() const
    {
        return _properties;
    }
} // namespace sb::cf