#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/AppSettingsConfiguration.hpp"
#include "SevenBit/Conf/CommandLineConfiguration.hpp"
#include "SevenBit/Conf/EnvironmentVarsConfiguration.hpp"
#include "SevenBit/Conf/IConfiguration.hpp"
#include "SevenBit/Conf/IObject.hpp"
#include "SevenBit/Conf/JsonConfiguration.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/JsonStreamConfiguration.hpp"
#include "SevenBit/Conf/KeyPerFileConfiguration.hpp"
#include "SevenBit/Conf/OptionsParserConfig.hpp"
#include "SevenBit/Conf/SettingsConfiguration.hpp"

namespace sb::cf
{
    struct IConfigurationBuilder
    {
        using Ptr = std::unique_ptr<IConfigurationBuilder>;

        virtual IConfigurationBuilder &add(IConfigurationSource::SPtr) = 0;

        virtual std::unordered_map<std::string, IObject::Ptr> &getProperties() = 0;

        virtual const std::unordered_map<std::string, IObject::Ptr> &getProperties() const = 0;

        virtual std::vector<IConfigurationSource::SPtr> &getSources() = 0;

        virtual const std::vector<IConfigurationSource::SPtr> &getSources() const = 0;

        virtual IConfiguration::Ptr build() = 0;

        virtual void clear() = 0;

        IConfigurationBuilder &addJsonFile(std::filesystem::path filePath, bool isOptional = false)
        {
            return add(JsonFileConfigurationSource::create(std::move(filePath), isOptional));
        }

        IConfigurationBuilder &addJsonStream(std::istringstream &stream)
        {
            return add(JsonStreamConfigurationSource::create(stream));
        }

        IConfigurationBuilder &addAppSettings(std::string envName = "")
        {
            return add(AppSettingsConfigurationSource::create(std::move(envName)));
        }

        IConfigurationBuilder &addEnvironmentVariables(std::string prefix = "", SettingParserConfig config = {})
        {
            return add(EnvironmentVarsConfigurationSource::create(std::move(prefix), std::move(config)));
        }

        IConfigurationBuilder &addCommandLine(int argc, char **argv, SettingParserConfig config = {})
        {
            return add(CommandLineConfigurationSource::create(argc, argv, std::move(config)));
        }

        IConfigurationBuilder &addCommandLine(std::vector<std::string_view> args, SettingParserConfig config = {})
        {
            return add(CommandLineConfigurationSource::create(std::move(args), std::move(config)));
        }

        IConfigurationBuilder &addJson(JsonObject object)
        {
            return add(JsonConfigurationSource::create(std::move(object)));
        }

        IConfigurationBuilder &addKeyPerFile(std::filesystem::path directoryPath, bool isOptional = false,
                                             std::string ignorePrefix = "")
        {
            return add(
                KeyPerFileConfigurationSource::create(std::move(directoryPath), isOptional, std::move(ignorePrefix)));
        }

        IConfigurationBuilder &addKeyPerFile(std::filesystem::path directoryPath, bool isOptional,
                                             std::function<bool(const std::filesystem::path &)> ignoreCondition)
        {
            return add(KeyPerFileConfigurationSource::create(std::move(directoryPath), isOptional,
                                                             std::move(ignoreCondition)));
        }

        IConfigurationBuilder &addSetting(std::string_view key, JsonValue value)
        {
            return addSettings({{key, std::move(value)}});
        }

        IConfigurationBuilder &addSettings(std::vector<std::pair<std::string_view, JsonValue>> settings)
        {
            return add(SettingsConfigurationSource::create(std::move(settings)));
        }

        virtual ~IConfigurationBuilder() = default;
    };
} // namespace sb::cf