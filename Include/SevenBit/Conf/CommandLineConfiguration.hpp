#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"

namespace sb::cf
{
    class EXPORT CommandLineConfigurationSource : public IConfigurationSource,
                                                  public std::enable_shared_from_this<CommandLineConfigurationSource>
    {
      private:
        std::vector<std::string_view> _args;
        details::SettingParser _parser;

        CommandLineConfigurationSource(std::vector<std::string_view> args, SettingParserConfig config);

      public:
        using Ptr = std::unique_ptr<CommandLineConfigurationSource>;
        using SPtr = std::shared_ptr<CommandLineConfigurationSource>;

        static SPtr create(int argc, const char *const *argv, SettingParserConfig config = {});

        static SPtr create(std::vector<std::string_view> args, SettingParserConfig config = {});

        const std::vector<std::string_view> &getArgs() const;

        const details::SettingParser &getOptionsParser() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };

    class EXPORT CommandLineConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        CommandLineConfigurationSource::SPtr _source;

      public:
        CommandLineConfigurationProvider(CommandLineConfigurationSource::SPtr source);

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/CommandLineConfiguration.hpp"
#endif