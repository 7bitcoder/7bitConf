#pragma once

#include <memory>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/Sources/ConfigurationProviderBase.hpp"

namespace sb::cf
{
    class EXPORT ChainedConfigurationSource : public IConfigurationSource
    {
      private:
        std::vector<IConfigurationSource::SPtr> _sources;

      public:
        using Ptr = std::unique_ptr<ChainedConfigurationSource>;
        using SPtr = std::shared_ptr<ChainedConfigurationSource>;

        explicit ChainedConfigurationSource(std::vector<IConfigurationSource::SPtr> sources = {});

        [[nodiscard]] static Ptr create(std::vector<IConfigurationSource::SPtr> sources = {});

        void add(IConfigurationSource::SPtr source);

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;

        [[nodiscard]] auto begin() { return _sources.begin(); }

        [[nodiscard]] auto end() { return _sources.end(); }
    };

    class EXPORT ChainedConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        std::vector<IConfigurationProvider::Ptr> _providers;

      public:
        explicit ChainedConfigurationProvider(std::vector<IConfigurationProvider::Ptr> providers);

        ChainedConfigurationProvider(const ChainedConfigurationProvider &) = delete;

        ChainedConfigurationProvider &operator=(const ChainedConfigurationProvider &) = delete;

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Sources/Impl/ChainedConfiguration.hpp"
#endif
