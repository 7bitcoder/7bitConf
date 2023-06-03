#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfiguration.hpp"
#include "SevenBit/Conf/IConfigurationBuilder.hpp"

namespace sb::cf
{
    EXPORT class ConfigurationBuilder : public IConfigurationBuilder
    {
      private:
        std::unordered_map<std::string, IObject::Ptr> _properties;

        std::vector<IConfigurationSource::SPtr> _sources;

      public:
        using Ptr = std::unique_ptr<ConfigurationBuilder>;

        ConfigurationBuilder(std::vector<IConfigurationSource::SPtr> = {});

        ConfigurationBuilder(ConfigurationBuilder &&) = default;
        ConfigurationBuilder(const ConfigurationBuilder &) = default;

        ConfigurationBuilder &operator=(ConfigurationBuilder &&) = default;
        ConfigurationBuilder &operator=(const ConfigurationBuilder &) = default;

        IConfigurationBuilder &add(IConfigurationSource::SPtr source) override;

        std::vector<IConfigurationSource::SPtr> &getSources() override;

        const std::vector<IConfigurationSource::SPtr> &getSources() const override;

        std::unordered_map<std::string, IObject::Ptr> &getProperties() override;

        const std::unordered_map<std::string, IObject::Ptr> &getProperties() const override;

        IConfiguration::Ptr build() override;

        void clear() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/ConfigurationBuilder.hpp"
#endif