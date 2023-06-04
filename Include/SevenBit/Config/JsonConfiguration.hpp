#pragma once

#include <memory>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/ConfigurationProviderBase.hpp"
#include "SevenBit/Config/IConfigurationSource.hpp"

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

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/JsonConfiguration.hpp"
#endif