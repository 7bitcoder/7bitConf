#pragma once

#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    template <class Source> class ConfigurationProviderBase : public IConfigurationProvider
    {
      protected:
        std::shared_ptr<Source> _source;

        JsonObject _configuration;

      public:
        ConfigurationProviderBase(std::shared_ptr<Source> source) : _source(std::move(source)) {}

        const JsonObject &getConfiguration() const override { return _configuration; }

        JsonObject &getConfiguration() override { return _configuration; }

        void clear() { _configuration.clear(); }

        void set(JsonObject configuration) { _configuration = std::move(configuration); }

        void setFrom(IConfigurationSource::SPtr source)
        {
            auto provider = source->build();
            provider->load();
            set(std::move(provider->getConfiguration()));
        }

        void update(JsonObject configuration) { JsonObjectExt::deepMerge(_configuration, std::move(configuration)); }

        void updateFrom(IConfigurationSource::SPtr source)
        {
            auto provider = source->build();
            provider->load();
            update(std::move(provider->getConfiguration()));
        }
    };
} // namespace sb::cf
