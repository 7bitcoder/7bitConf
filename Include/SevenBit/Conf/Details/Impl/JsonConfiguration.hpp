#pragma once

#include "SevenBit/Conf/JsonConfiguration.hpp"

namespace sb::cf
{
    INLINE JsonConfigurationSource::JsonConfigurationSource(JsonObject configuration)
        : _configuration(std::move(configuration))
    {
    }

    INLINE JsonConfigurationSource::SPtr JsonConfigurationSource::create(JsonObject configuration)
    {
        return JsonConfigurationSource::SPtr(new JsonConfigurationSource{std::move(configuration)});
    }

    INLINE const JsonObject &JsonConfigurationSource::getJson() const { return _configuration; }

    INLINE IConfigurationProvider::Ptr JsonConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<JsonConfigurationProvider>(shared_from_this());
    }

    INLINE JsonConfigurationProvider::JsonConfigurationProvider(JsonConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
    }

    INLINE void JsonConfigurationProvider::load() { set(_source->getJson()); }
} // namespace sb::cf