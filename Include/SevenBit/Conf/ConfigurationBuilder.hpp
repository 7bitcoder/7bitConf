#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationBuilder.hpp"

namespace sb::cf
{
    class EXPORT ConfigurationBuilder : public IConfigurationBuilder
    {
      private:
        std::unordered_map<std::string, IObject::SPtr> _properties;

        std::vector<IConfigurationSource::SPtr> _sources;

      public:
        using Ptr = std::unique_ptr<ConfigurationBuilder>;

        explicit ConfigurationBuilder(std::vector<IConfigurationSource::SPtr> = {});

        ConfigurationBuilder(ConfigurationBuilder &&) noexcept = default;
        ConfigurationBuilder(const ConfigurationBuilder &) = default;

        ConfigurationBuilder &operator=(ConfigurationBuilder &&) = default;
        ConfigurationBuilder &operator=(const ConfigurationBuilder &) = default;

        IConfigurationBuilder &add(IConfigurationSource::SPtr source) override;

        [[nodiscard]] std::vector<IConfigurationSource::SPtr> &getSources() override;

        [[nodiscard]] const std::vector<IConfigurationSource::SPtr> &getSources() const override;

        [[nodiscard]] std::unordered_map<std::string, IObject::SPtr> &getProperties() override;

        [[nodiscard]] const std::unordered_map<std::string, IObject::SPtr> &getProperties() const override;

        IConfiguration::Ptr build() override;

        void clear() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Impl/ConfigurationBuilder.hpp"
#endif
