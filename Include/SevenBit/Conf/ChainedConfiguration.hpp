#pragma once

#include <memory>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class ChainedConfigurationSource : public IConfigurationSource,
                                              public std::enable_shared_from_this<ChainedConfigurationSource>
    {
      private:
        std::vector<IConfigurationSource::SPtr> _sources;

        ChainedConfigurationSource(std::vector<IConfigurationSource::SPtr> sources = {});

      public:
        using Ptr = std::unique_ptr<ChainedConfigurationSource>;
        using SPtr = std::shared_ptr<ChainedConfigurationSource>;

        static SPtr create(std::vector<IConfigurationSource::SPtr> sources = {});

        void add(IConfigurationSource::SPtr source);

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;

        auto begin() { return _sources.begin(); }

        auto end() { return _sources.end(); }
    };

    EXPORT class ChainedConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        std::vector<IConfigurationProvider::Ptr> _providers;

      public:
        ChainedConfigurationProvider(std::vector<IConfigurationProvider::Ptr> providers);

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ChainedConfiguration.hpp"
#endif