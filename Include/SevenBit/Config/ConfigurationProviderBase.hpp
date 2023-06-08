#pragma once

#include <memory>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/IConfigurationProvider.hpp"

namespace sb::cf
{
    class EXPORT ConfigurationProviderBase : public IConfigurationProvider
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

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/ConfigurationProviderBase.hpp"
#endif