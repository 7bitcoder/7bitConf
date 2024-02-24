#pragma once

#include <istream>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/Sources/ConfigurationProviderBase.hpp"

namespace sb::cf
{
    class EXPORT JsonStreamConfigurationSource : public IConfigurationSource,
                                                 public std::enable_shared_from_this<JsonStreamConfigurationSource>
    {
      private:
        std::istream &_stream;

        explicit JsonStreamConfigurationSource(std::istream &stream);

      public:
        using Ptr = std::unique_ptr<JsonStreamConfigurationSource>;
        using SPtr = std::shared_ptr<JsonStreamConfigurationSource>;

        static SPtr create(std::istream &stream);

        [[nodiscard]] std::istream &getStream();

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };

    class EXPORT JsonStreamConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        JsonStreamConfigurationSource::SPtr _source;

      public:
        explicit JsonStreamConfigurationProvider(JsonStreamConfigurationSource::SPtr source);

        void load() override;

      private:
        [[nodiscard]] JsonObject getJsonFromStream();
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Sources/Impl/JsonStreamConfiguration.hpp"
#endif
