#pragma once

#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/MapConfiguration.hpp"

namespace sb::cf
{
    INLINE MapConfigurationSource::MapConfigurationSource(IConfigurationSource::SPtr source,
                                                          std::function<JsonObject(JsonObject)> mapFcn)
        : _source(std::move(source)), _mapFcn(std::move(mapFcn))
    {
        details::utils::assertPtr(_source);
    }

    INLINE MapConfigurationSource::SPtr MapConfigurationSource::create(IConfigurationSource::SPtr source,
                                                                       std::function<JsonObject(JsonObject)> mapFcn)
    {
        return MapConfigurationSource::SPtr(new MapConfigurationSource{std::move(source), std::move(mapFcn)});
    }

    INLINE const std::function<JsonObject(JsonObject)> &MapConfigurationSource::getMapFcn() const { return _mapFcn; }

    INLINE IConfigurationProvider::Ptr MapConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<MapConfigurationProvider>(shared_from_this(), _source->build(builder));
    }

    INLINE MapConfigurationProvider::MapConfigurationProvider(MapConfigurationSource::SPtr source,
                                                              IConfigurationProvider::Ptr innerProvider)
        : _source(std::move(source)), _innerProvider(std::move(innerProvider))
    {
        details::utils::assertPtr(_source);
        details::utils::assertPtr(_innerProvider);
    }

    INLINE void MapConfigurationProvider::load()
    {
        details::utils::assertPtr(_source);
        details::utils::assertPtr(_innerProvider);
        _innerProvider->load();
        set(_source->getMapFcn()(std::move(_innerProvider->getConfiguration())));
    }
} // namespace sb::cf