#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/Sources/ConfigurationProviderBase.hpp"

namespace sb::cf
{
    class EXPORT JsonConfigurationSource : public IConfigurationSource,
                                           public std::enable_shared_from_this<JsonConfigurationSource>
    {
      private:
        JsonObject _configuration;

        explicit JsonConfigurationSource(JsonObject configuration);

      public:
        using Ptr = std::unique_ptr<JsonConfigurationSource>;
        using SPtr = std::shared_ptr<JsonConfigurationSource>;

        [[nodiscard]] static SPtr create(JsonObject configuration);

        [[nodiscard]] const JsonObject &getJson() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };

    class EXPORT JsonConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        JsonConfigurationSource::SPtr _source;

      public:
        explicit JsonConfigurationProvider(JsonConfigurationSource::SPtr source);

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Sources/Impl/JsonConfiguration.hpp"
#endif
