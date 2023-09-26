#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationProvider.hpp"

namespace sb::cf
{
    class EXPORT ConfigurationProviderBase : public IConfigurationProvider
    {
      protected:
        JsonObject _configuration;

      public:
        const JsonObject &getConfiguration() const override;

        JsonObject &getConfiguration() override;

      protected:
        void clear();

        void set(JsonObject configuration);

        void update(JsonObject configuration);

        void update(const std::vector<std::string_view> &keys, JsonValue value);

        void throwNullSourceException() const;

        void throwNullProviderException() const;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ConfigurationProviderBase.hpp"
#endif