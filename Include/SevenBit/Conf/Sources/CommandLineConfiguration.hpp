#pragma once

#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"
#include "SevenBit/Conf/SettingParserBuilder.hpp"
#include "SevenBit/Conf/Sources/ConfigurationProviderBase.hpp"

namespace sb::cf
{
    class EXPORT CommandLineConfigurationSource : public IConfigurationSource,
                                                  public std::enable_shared_from_this<CommandLineConfigurationSource>
    {
      private:
        std::vector<std::string_view> _args;
        ISettingsParser::Ptr _parser;

        CommandLineConfigurationSource(std::vector<std::string_view> args, ISettingsParser::Ptr parser);

      public:
        using Ptr = std::unique_ptr<CommandLineConfigurationSource>;
        using SPtr = std::shared_ptr<CommandLineConfigurationSource>;

        [[nodiscard]] static SPtr create(int argc, const char *const *argv, ISettingsParser::Ptr parser);

        [[nodiscard]] static SPtr create(std::vector<std::string_view> args, ISettingsParser::Ptr parser);

        [[nodiscard]] const std::vector<std::string_view> &getArgs() const;

        [[nodiscard]] const ISettingsParser &getSettingsParser() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };

    class EXPORT CommandLineConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        CommandLineConfigurationSource::SPtr _source;

      public:
        explicit CommandLineConfigurationProvider(CommandLineConfigurationSource::SPtr source);

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Sources/Impl/CommandLineConfiguration.hpp"
#endif
