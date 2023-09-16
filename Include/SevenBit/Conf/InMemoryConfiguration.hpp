#pragma once

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/Details/SettingReader.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    class EXPORT InMemoryConfigurationSource : public IConfigurationSource,
                                               public std::enable_shared_from_this<InMemoryConfigurationSource>
    {
      private:
        std::vector<std::pair<std::string_view, JsonValue>> _settings;

        InMemoryConfigurationSource(std::vector<std::pair<std::string_view, JsonValue>> settings);

      public:
        using Ptr = std::unique_ptr<InMemoryConfigurationSource>;
        using SPtr = std::shared_ptr<InMemoryConfigurationSource>;

        static SPtr create(std::vector<std::pair<std::string_view, JsonValue>> settings);

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;

        auto begin() { return _settings.begin(); }

        auto end() { return _settings.end(); }
    };

    class EXPORT InMemoryConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        InMemoryConfigurationSource::SPtr _source;

      public:
        InMemoryConfigurationProvider(InMemoryConfigurationSource::SPtr source);

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/InMemoryConfiguration.hpp"
#endif