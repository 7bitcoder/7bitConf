#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    class EXPORT MapConfigurationSource : public IConfigurationSource,
                                          public std::enable_shared_from_this<MapConfigurationSource>
    {
      private:
        IConfigurationSource::SPtr _source;
        std::function<JsonObject(JsonObject)> _mapFcn;

        MapConfigurationSource(IConfigurationSource::SPtr source, std::function<JsonObject(JsonObject)> mapFcn);

      public:
        using Ptr = std::unique_ptr<MapConfigurationSource>;
        using SPtr = std::shared_ptr<MapConfigurationSource>;

        static SPtr create(IConfigurationSource::SPtr source, std::function<JsonObject(JsonObject)> mapFcn);

        const std::function<JsonObject(JsonObject)> &getMapFcn() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };

    class EXPORT MapConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        MapConfigurationSource::SPtr _source;
        IConfigurationProvider::Ptr _innerProvider;

      public:
        MapConfigurationProvider(MapConfigurationSource::SPtr source, IConfigurationProvider::Ptr innerProvider);

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/MapConfiguration.hpp"
#endif