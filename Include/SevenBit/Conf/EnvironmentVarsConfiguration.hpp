#pragma once

#include <memory>
#include <string>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/ISettingParser.hpp"
#include "SevenBit/Conf/SettingParserBuilder.hpp"

namespace sb::cf
{
    class EXPORT EnvironmentVarsConfigurationSource
        : public IConfigurationSource,
          public std::enable_shared_from_this<EnvironmentVarsConfigurationSource>
    {
      private:
        std::string _prefix;
        ISettingParser::Ptr _parser;

        EnvironmentVarsConfigurationSource(std::string prefix, ISettingParser::Ptr parser);

      public:
        using Ptr = std::unique_ptr<EnvironmentVarsConfigurationSource>;
        using SPtr = std::shared_ptr<EnvironmentVarsConfigurationSource>;

        [[nodiscard]] static SPtr create(std::string prefix,
                                         ISettingParser::Ptr parser = SettingParserBuilder{}.build());

        [[nodiscard]] const std::string &getPrefix();

        [[nodiscard]] const ISettingParser &getSettingParser();

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };

    class EXPORT EnvironmentVarsConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        EnvironmentVarsConfigurationSource::SPtr _source;

      public:
        explicit EnvironmentVarsConfigurationProvider(EnvironmentVarsConfigurationSource::SPtr source);

        void load() override;

      private:
        [[nodiscard]] std::vector<std::string_view> getEnvVars();
    };

} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/EnvironmentVarsConfiguration.hpp"
#endif