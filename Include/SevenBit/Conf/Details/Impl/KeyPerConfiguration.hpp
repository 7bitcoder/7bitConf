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

    INLINE void KeyPerConfigurationSource::add(const std::string &key, IConfigurationSource::Ptr source)
    {
        _keySources.emplace_back(key, std::move(source));
    }

    INLINE IConfigurationProvider::Ptr KeyPerConfigurationSource::build() const
    {
        return std::make_unique<KeyPerConfigurationProvider>(*this);
    }

    INLINE KeyPerConfigurationProvider::KeyPerConfigurationProvider(KeyPerConfigurationSource source)
        : _source(std::move(source))
    {
    }

    INLINE void KeyPerConfigurationProvider::load()
    {
        _configuration.clear();
        for (auto &[key, source] : _source)
        {
            auto provider = source->build();
            provider->load();
            _configuration[key] = provider->get();
        }
    }

    INLINE const JsonObject &KeyPerConfigurationProvider::get() const { return _configuration; }
} // namespace sb::cf