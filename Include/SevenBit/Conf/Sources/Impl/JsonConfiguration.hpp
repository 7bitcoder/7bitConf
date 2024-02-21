#pragma once

#include "SevenBit/Conf/Details/Require.hpp"
#include "SevenBit/Conf/Sources/JsonConfiguration.hpp"

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
        details::Require::notNull(_source);
    }

    INLINE void JsonConfigurationProvider::load()
    {
        auto copy = _source->getJson();
        set(std::move(copy));
    }
} // namespace sb::cf
