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
        [[nodiscard]] const JsonObject &getConfiguration() const override;

        [[nodiscard]] JsonObject &getConfiguration();

      protected:
        void clear();

        void set(const JsonObject &configuration);
        void set(JsonObject &&configuration);

        void update(const JsonObject &configuration);
        void update(JsonObject &&configuration);

        void updateWith(std::string_view key, const JsonValue &value);
        void updateWith(std::string_view key, JsonValue &&value);

        void updateWith(const std::vector<std::string_view> &keys, const JsonValue &value);
        void updateWith(const std::vector<std::string_view> &keys, JsonValue &&value);
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Sources/Impl/ConfigurationProviderBase.hpp"
#endif
