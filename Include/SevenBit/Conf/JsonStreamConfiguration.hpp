#pragma once
#include <ostream>
#include <sstream>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/IConfigurationProvider.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class JsonStreamConfigurationSource : public IConfigurationSource
    {
      private:
        std::istream &_stream;

      public:
        JsonStreamConfigurationSource(std::istream &stream);

        std::istream &getStream();

        IConfigurationProvider::Ptr build() const override;
    };

    EXPORT class JsonStreamConfigurationProvider : public IConfigurationProvider
    {
      private:
        JsonStreamConfigurationSource _source;

        JsonObject _configuration;

      public:
        JsonStreamConfigurationProvider(JsonStreamConfigurationSource source);

        void load() override;

        const JsonObject &get() const override;

      private:
        JsonObject getJsonFromFile();
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonStreamConfiguration.hpp"
#endif