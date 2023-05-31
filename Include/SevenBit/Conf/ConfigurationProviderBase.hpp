#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    class ConfigurationProviderBase : public IConfigurationProvider
    {
      protected:
        JsonObject _configuration;

      public:
        const JsonObject &getConfiguration() const override;

        JsonObject &getConfiguration() override;

        void clear();

        void set(JsonObject configuration);

        void update(JsonObject configuration);
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ConfigurationProviderBase.hpp"
#endif