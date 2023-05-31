#pragma once

#include <memory>
#include <string>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class EnvironmentVarsConfigurationSource
        : public IConfigurationSource,
          public std::enable_shared_from_this<EnvironmentVarsConfigurationSource>
    {
      private:
        std::string _prefix;
        details::SettingParser _parser;

        EnvironmentVarsConfigurationSource(std::string prefix, SettingParserConfig parserConfig);

      public:
        using Ptr = std::unique_ptr<EnvironmentVarsConfigurationSource>;
        using SPtr = std::shared_ptr<EnvironmentVarsConfigurationSource>;

        static SPtr create(std::string prefix = "", SettingParserConfig parserCfg = {});

        const std::string &getPrefix();

        const details::SettingParser &getSettingParser();

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };

    EXPORT class EnvironmentVarsConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        EnvironmentVarsConfigurationSource::SPtr _source;

      public:
        EnvironmentVarsConfigurationProvider(EnvironmentVarsConfigurationSource::SPtr source);

        void load() override;

      private:
        std::vector<std::string_view> getEnvVars();
    };

} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/EnvironmentVarsConfiguration.hpp"
#endif