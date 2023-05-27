#pragma once

#include <memory>

#include "SevenBit/Conf/JsonConfiguration.hpp"

namespace sb::cf
{
    INLINE JsonConfigurationProvider::JsonConfigurationProvider(JsonConfigurationSource source) : _source(source) {}

    INLINE void JsonConfigurationProvider::load() {}

    INLINE const JsonObject &JsonConfigurationProvider::get() const { return _source.getJson(); }

    INLINE JsonConfigurationSource::JsonConfigurationSource(JsonObject configuration)
        : _configuration(std::move(configuration))
    {
    }

    INLINE const JsonObject &JsonConfigurationSource::getJson() const { return _configuration; }

    INLINE IConfigurationProvider::Ptr JsonConfigurationSource::build() const
    {
        return std::make_unique<JsonConfigurationProvider>(*this);
    }
} // namespace sb::cf