#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/IConfiguration.hpp"
#include "SevenBit/Config/IConfigurationBuilder.hpp"

namespace sb::cf
{
    class EXPORT ConfigurationBuilder : public IConfigurationBuilder
    {
      private:
        std::unordered_map<std::string, IObject::SPtr> _properties;

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

        std::unordered_map<std::string, IObject::SPtr> &getProperties() override;

        const std::unordered_map<std::string, IObject::SPtr> &getProperties() const override;

        IConfiguration::Ptr build() override;

        void clear() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/ConfigurationBuilder.hpp"
#endif