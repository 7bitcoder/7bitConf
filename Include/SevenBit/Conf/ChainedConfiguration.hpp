#pragma once

#include <filesystem>
#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class ChainedConfigurationSource : public IConfigurationSource
    {
      private:
        std::vector<IConfigurationSource::SPtr> _sources;

      public:
        ChainedConfigurationSource(std::vector<IConfigurationSource::SPtr> sources = {});

        void add(IConfigurationSource::Ptr source);

        IConfigurationProvider::Ptr build() const override;

        auto begin() const { return _sources.begin(); }

        auto end() const { return _sources.end(); }
    };

    EXPORT class ChainedConfigurationProvider : public IConfigurationProvider
    {
      private:
        ChainedConfigurationSource _source;

        JsonObject _configuration;

      public:
        ChainedConfigurationProvider(ChainedConfigurationSource source);

        void load() override;

        const JsonObject &get() const override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ChainedConfiguration.hpp"
#endif