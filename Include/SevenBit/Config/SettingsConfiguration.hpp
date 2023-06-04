#pragma once

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/ConfigurationProviderBase.hpp"
#include "SevenBit/Config/Details/SettingParser.hpp"
#include "SevenBit/Config/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class SettingsConfigurationSource : public IConfigurationSource,
                                               public std::enable_shared_from_this<SettingsConfigurationSource>
    {
      private:
        std::vector<std::pair<std::string_view, JsonValue>> _settings;
        details::SettingParser _settingsParser;

        SettingsConfigurationSource(std::vector<std::pair<std::string_view, JsonValue>> settings,
                                    SettingParserConfig parserCfg);

      public:
        using Ptr = std::unique_ptr<SettingsConfigurationSource>;
        using SPtr = std::shared_ptr<SettingsConfigurationSource>;

        static SPtr create(std::vector<std::pair<std::string_view, JsonValue>> settings,
                           SettingParserConfig parserCfg = {});

        const details::SettingParser &getSettingParser() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;

        auto begin() { return _settings.begin(); }

        auto end() { return _settings.end(); }
    };

    EXPORT class SettingsConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        SettingsConfigurationSource::SPtr _source;

      public:
        SettingsConfigurationProvider(SettingsConfigurationSource::SPtr source);

        void load() override;
    };
} // namespace sb::cf

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/SettingsConfiguration.hpp"
#endif