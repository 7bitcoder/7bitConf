#pragma once

#include <memory>
#include <string>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/Details/OptionsParser.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class EnvironmentVarsConfigurationSource
        : public IConfigurationSource,
          public std::enable_shared_from_this<EnvironmentVarsConfigurationSource>
    {
      private:
        std::string _prefix;
        OptionsParser _parser;

        EnvironmentVarsConfigurationSource(std::string prefix, OptionsParserConfig parserConfig);

      public:
        using Ptr = std::unique_ptr<EnvironmentVarsConfigurationSource>;
        using SPtr = std::shared_ptr<EnvironmentVarsConfigurationSource>;

        static SPtr create(std::string prefix = "", OptionsParserConfig parserCfg = {});

        const std::string &getPrefix();

        const OptionsParser &getOptionsParser();

        IConfigurationProvider::Ptr build() override;
    };

    EXPORT class EnvironmentVarsConfigurationProvider
        : public ConfigurationProviderBase<EnvironmentVarsConfigurationSource>
    {
      public:
        using ConfigurationProviderBase<EnvironmentVarsConfigurationSource>::ConfigurationProviderBase;

        void load() override;

      private:
        std::vector<std::string_view> getEnvVars();
    };

} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/EnvironmentVarsConfiguration.hpp"
#endif