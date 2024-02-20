#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/CommandLineParserBuilder.hpp"
#include "SevenBit/Conf/CommandLineParserConfig.hpp"
#include "SevenBit/Conf/IConfiguration.hpp"
#include "SevenBit/Conf/IObject.hpp"
#include "SevenBit/Conf/SettingParserBuilder.hpp"
#include "SevenBit/Conf/Sources/AppSettingsConfiguration.hpp"
#include "SevenBit/Conf/Sources/CommandLineConfiguration.hpp"
#include "SevenBit/Conf/Sources/EnvironmentVarsConfiguration.hpp"
#include "SevenBit/Conf/Sources/InMemoryConfiguration.hpp"
#include "SevenBit/Conf/Sources/JsonConfiguration.hpp"
#include "SevenBit/Conf/Sources/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/Sources/JsonStreamConfiguration.hpp"
#include "SevenBit/Conf/Sources/KeyPerFileConfiguration.hpp"

namespace sb::cf
{
    struct IConfigurationBuilder
    {
        using Ptr = std::unique_ptr<IConfigurationBuilder>;

        virtual IConfigurationBuilder &add(IConfigurationSource::SPtr) = 0;

        [[nodiscard]] virtual std::unordered_map<std::string, IObject::SPtr> &getProperties() = 0;

        [[nodiscard]] virtual const std::unordered_map<std::string, IObject::SPtr> &getProperties() const = 0;

        [[nodiscard]] virtual std::vector<IConfigurationSource::SPtr> &getSources() = 0;

        [[nodiscard]] virtual const std::vector<IConfigurationSource::SPtr> &getSources() const = 0;

        virtual IConfiguration::Ptr build() = 0;

        virtual void clear() = 0;

        template <class TFactory> IConfigurationBuilder &addFrom(TFactory factory) { return add(factory(*this)); }

        IConfigurationBuilder &addJsonFile(std::filesystem::path filePath)
        {
            return addJsonFile(std::move(filePath), false);
        }

        IConfigurationBuilder &addJsonFile(std::filesystem::path filePath, bool isOptional)
        {
            return add(JsonFileConfigurationSource::create(std::move(filePath), isOptional));
        }

        IConfigurationBuilder &addJsonStream(std::istream &stream)
        {
            return add(JsonStreamConfigurationSource::create(stream));
        }

        IConfigurationBuilder &addAppSettings() { return addAppSettings(""); }

        IConfigurationBuilder &addAppSettings(std::string environmentName)
        {
            return add(AppSettingsConfigurationSource::create(std::move(environmentName)));
        }

        IConfigurationBuilder &addEnvironmentVariables() { return addEnvironmentVariables("", SettingParserConfig{}); }

        IConfigurationBuilder &addEnvironmentVariables(std::string prefix)
        {
            return addEnvironmentVariables(std::move(prefix), SettingParserConfig{});
        }

        IConfigurationBuilder &addEnvironmentVariables(std::string prefix, SettingParserConfig config)
        {
            return addEnvironmentVariables(std::move(prefix),
                                           SettingParserBuilder{}.useConfig(std::move(config)).build());
        }

        IConfigurationBuilder &addEnvironmentVariables(std::string prefix, ISettingParser::Ptr parser)
        {
            return add(EnvironmentVarsConfigurationSource::create(std::move(prefix), std::move(parser)));
        }

        IConfigurationBuilder &addCommandLine(int argc, char *const *const argv)
        {
            return addCommandLine(argc, argv, CommandLineParserConfig{});
        }

        IConfigurationBuilder &addCommandLine(std::vector<std::string_view> args)
        {
            return addCommandLine(std::move(args), CommandLineParserConfig{});
        }

        IConfigurationBuilder &addCommandLine(int argc, char *const *const argv, CommandLineParserConfig config)
        {
            return addCommandLine(argc, argv, CommandLineParserBuilder{}.useConfig(std::move(config)).build());
        }

        IConfigurationBuilder &addCommandLine(std::vector<std::string_view> args, CommandLineParserConfig config)
        {
            return addCommandLine(std::move(args), CommandLineParserBuilder{}.useConfig(std::move(config)).build());
        }

        IConfigurationBuilder &addCommandLine(int argc, char *const *const argv, ISettingsParser::Ptr parser)
        {
            return add(CommandLineConfigurationSource::create(argc, argv, std::move(parser)));
        }

        IConfigurationBuilder &addCommandLine(std::vector<std::string_view> args, ISettingsParser::Ptr parser)
        {
            return add(CommandLineConfigurationSource::create(std::move(args), std::move(parser)));
        }

        IConfigurationBuilder &addJson(JsonObject object)
        {
            return add(JsonConfigurationSource::create(std::move(object)));
        }

        IConfigurationBuilder &addKeyPerFile(std::filesystem::path directoryPath)
        {
            return addKeyPerFile(std::move(directoryPath), false, "");
        }

        IConfigurationBuilder &addKeyPerFile(std::filesystem::path directoryPath, bool isOptional)
        {
            return addKeyPerFile(std::move(directoryPath), isOptional, "");
        }

        IConfigurationBuilder &addKeyPerFile(std::filesystem::path directoryPath, bool isOptional,
                                             std::string ignorePrefix)
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

        IConfigurationBuilder &AddInMemory(std::string_view key, JsonValue value)
        {
            return AddInMemory({{key, std::move(value)}});
        }

        IConfigurationBuilder &AddInMemory(std::vector<std::pair<std::string_view, JsonValue>> settings)
        {
            return add(InMemoryConfigurationSource::create(std::move(settings)));
        }

        IConfigurationBuilder &addDefault(int argc, char *const *const argv)
        {
            return addAppSettings().addEnvironmentVariables().addCommandLine(argc, argv);
        }

        virtual ~IConfigurationBuilder() = default;
    };
} // namespace sb::cf
