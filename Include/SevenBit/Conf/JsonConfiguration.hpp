#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class JsonConfigurationSource : public IConfigurationSource
    {
      private:
        JsonObject _configuration;

      public:
        JsonConfigurationSource(JsonObject configuration);

        const JsonObject &getJson() const;

        IConfigurationProvider::Ptr build() const override;
    };

    EXPORT class JsonConfigurationProvider : public IConfigurationProvider
    {
      private:
        JsonConfigurationSource _source;

      public:
        JsonConfigurationProvider(JsonConfigurationSource source);

        void load() override;

        const JsonObject &get() const override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonConfiguration.hpp"
#endif