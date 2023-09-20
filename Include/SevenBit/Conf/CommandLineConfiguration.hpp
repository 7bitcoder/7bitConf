#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/SettingParserBuilder.hpp"
#include "SevenBit/Conf/SettingParserConfig.hpp"

namespace sb::cf
{
    class EXPORT CommandLineConfigurationSource : public IConfigurationSource,
                                                  public std::enable_shared_from_this<CommandLineConfigurationSource>
    {
      private:
        std::vector<std::string_view> _args;
        ISettingParser::Ptr _parser;

        CommandLineConfigurationSource(std::vector<std::string_view> args, ISettingParser::Ptr parser);

      public:
        using Ptr = std::unique_ptr<CommandLineConfigurationSource>;
        using SPtr = std::shared_ptr<CommandLineConfigurationSource>;

        static SPtr create(int argc, const char *const *argv,
                           ISettingParser::Ptr parser = SettingParserBuilder{}.build());

        static SPtr create(std::vector<std::string_view> args,
                           ISettingParser::Ptr parser = SettingParserBuilder{}.build());

        const std::vector<std::string_view> &getArgs() const;

        const ISettingParser &getSettingParser() const;

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