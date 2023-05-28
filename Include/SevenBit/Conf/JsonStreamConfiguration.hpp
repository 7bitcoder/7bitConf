#pragma once
#include <ostream>
#include <sstream>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class JsonStreamConfigurationSource : public IConfigurationSource,
                                                 public std::enable_shared_from_this<JsonStreamConfigurationSource>
    {
      private:
        std::istream &_stream;

        JsonStreamConfigurationSource(std::istream &stream);

      public:
        using Ptr = std::unique_ptr<JsonStreamConfigurationSource>;
        using SPtr = std::shared_ptr<JsonStreamConfigurationSource>;

        static SPtr create(std::istream &stream);

        std::istream &getStream();

        IConfigurationProvider::Ptr build() override;
    };

    EXPORT class JsonStreamConfigurationProvider : public ConfigurationProviderBase<JsonStreamConfigurationSource>
    {
      public:
        using ConfigurationProviderBase<JsonStreamConfigurationSource>::ConfigurationProviderBase;

        void load() override;

      private:
        JsonObject getJsonFromStream();
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonStreamConfiguration.hpp"
#endif