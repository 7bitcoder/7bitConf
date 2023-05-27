#pragma once

#include <filesystem>
#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class KeyPerConfigurationSource : public IConfigurationSource
    {
      private:
        std::vector<std::pair<std::string, IConfigurationSource::SPtr>> _keySources;

      public:
        KeyPerConfigurationSource(std::vector<std::pair<std::string, IConfigurationSource::SPtr>> keySources = {});

        void add(const std::string &key, IConfigurationSource::Ptr source);

        IConfigurationProvider::Ptr build() const override;

        auto begin() const { return _keySources.begin(); }

        auto end() const { return _keySources.end(); }
    };

    EXPORT class KeyPerConfigurationProvider : public IConfigurationProvider
    {
      private:
        KeyPerConfigurationSource _source;

        JsonObject _configuration;

      public:
        KeyPerConfigurationProvider(KeyPerConfigurationSource source);

        void load() override;

        const JsonObject &get() const override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/KeyPerConfiguration.hpp"
#endif