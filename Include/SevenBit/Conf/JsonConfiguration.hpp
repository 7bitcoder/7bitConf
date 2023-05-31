#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class JsonConfigurationSource : public IConfigurationSource,
                                           public std::enable_shared_from_this<JsonConfigurationSource>
    {
      private:
        JsonObject _configuration;

        JsonConfigurationSource(JsonObject configuration);

      public:
        using Ptr = std::unique_ptr<JsonConfigurationSource>;
        using SPtr = std::shared_ptr<JsonConfigurationSource>;

        static SPtr create(JsonObject configuration);

        const JsonObject &getJson() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };

    EXPORT class JsonConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        JsonConfigurationSource::SPtr _source;

      public:
        JsonConfigurationProvider(JsonConfigurationSource::SPtr source);

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonConfiguration.hpp"
#endif