#pragma once

#include <string>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/OptionsParser.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class EnvironmentVarsConfigurationSource : public IConfigurationSource
    {
      private:
        std::string _prefix;
        OptionsParser _parser;

      public:
        EnvironmentVarsConfigurationSource(std::string prefix = "", OptionsParserCfg parserCfg = {});

        const std::string &getPrefix();

        const OptionsParser &getOptionsParser();

        IConfigurationProvider::Ptr build() const override;
    };

    EXPORT class EnvironmentVarsConfigurationProvider : public IConfigurationProvider
    {
      private:
        EnvironmentVarsConfigurationSource _source;

        JsonObject _configuration;

      public:
        EnvironmentVarsConfigurationProvider(EnvironmentVarsConfigurationSource source);

        void load() override;

        const JsonObject &get() const override;

      private:
        std::vector<std::string_view> getEnvVars();
    };

} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/EnvironmentVarsConfiguration.hpp"
#endif