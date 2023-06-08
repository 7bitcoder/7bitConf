#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/ConfigurationProviderBase.hpp"
#include "SevenBit/Config/Details/SettingParser.hpp"
#include "SevenBit/Config/IConfigurationSource.hpp"
#include "SevenBit/Config/OptionsParserConfig.hpp"

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

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/CommandLineConfiguration.hpp"
#endif