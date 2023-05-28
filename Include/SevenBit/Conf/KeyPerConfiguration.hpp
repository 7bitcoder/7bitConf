#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class KeyPerConfigurationSource : public IConfigurationSource,
                                             public std::enable_shared_from_this<KeyPerConfigurationSource>
    {
      private:
        std::vector<std::pair<std::string, IConfigurationSource::SPtr>> _keySources;

        KeyPerConfigurationSource(std::vector<std::pair<std::string, IConfigurationSource::SPtr>> keySources);

      public:
        using Ptr = std::unique_ptr<KeyPerConfigurationSource>;
        using SPtr = std::shared_ptr<KeyPerConfigurationSource>;

        static SPtr create(std::vector<std::pair<std::string, IConfigurationSource::SPtr>> keySources = {});

        void add(const std::string &key, IConfigurationSource::SPtr source);

        IConfigurationProvider::Ptr build() override;

        auto begin() { return _keySources.begin(); }

        auto end() { return _keySources.end(); }
    };

    EXPORT class KeyPerConfigurationProvider : public ConfigurationProviderBase<KeyPerConfigurationSource>
    {
      public:
        using ConfigurationProviderBase<KeyPerConfigurationSource>::ConfigurationProviderBase;

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/KeyPerConfiguration.hpp"
#endif