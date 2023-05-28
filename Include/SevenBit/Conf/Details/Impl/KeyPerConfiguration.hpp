#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/KeyPerConfiguration.hpp"

namespace sb::cf
{
    INLINE KeyPerConfigurationSource::KeyPerConfigurationSource(
        std::vector<std::pair<std::string, IConfigurationSource::SPtr>> keySources)
        : _keySources{std::move(keySources)}
    {
    }

    INLINE KeyPerConfigurationSource::SPtr KeyPerConfigurationSource::create(
        std::vector<std::pair<std::string, IConfigurationSource::SPtr>> keySources)
    {
        return KeyPerConfigurationSource::SPtr{new KeyPerConfigurationSource{std::move(keySources)}};
    }

    INLINE void KeyPerConfigurationSource::add(const std::string &key, IConfigurationSource::SPtr source)
    {
        _keySources.emplace_back(key, std::move(source));
    }

    INLINE IConfigurationProvider::Ptr KeyPerConfigurationSource::build()
    {
        return std::make_unique<KeyPerConfigurationProvider>(shared_from_this());
    }

    INLINE void KeyPerConfigurationProvider::load()
    {
        clear();
        for (auto &[key, source] : *_source)
        {
            auto provider = source->build();
            provider->load();
            _configuration[key] = std::move(provider->getConfiguration());
        }
    }
} // namespace sb::cf