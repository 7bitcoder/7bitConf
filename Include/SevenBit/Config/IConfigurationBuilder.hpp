#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/AppSettingsConfiguration.hpp"
#include "SevenBit/Config/CommandLineConfiguration.hpp"
#include "SevenBit/Config/EnvironmentVarsConfiguration.hpp"
#include "SevenBit/Config/IConfiguration.hpp"
#include "SevenBit/Config/IObject.hpp"
#include "SevenBit/Config/InMemoryConfiguration.hpp"
#include "SevenBit/Config/JsonConfiguration.hpp"
#include "SevenBit/Config/JsonFileConfiguration.hpp"
#include "SevenBit/Config/JsonStreamConfiguration.hpp"
#include "SevenBit/Config/KeyPerFileConfiguration.hpp"
#include "SevenBit/Config/OptionsParserConfig.hpp"

namespace sb::cf
{
    struct IConfigurationBuilder
    {
        using Ptr = std::unique_ptr<IConfigurationBuilder>;

        virtual IConfigurationBuilder &add(IConfigurationSource::SPtr) = 0;

        virtual std::unordered_map<std::string, IObject::SPtr> &getProperties() = 0;

        virtual const std::unordered_map<std::string, IObject::SPtr> &getProperties() const = 0;

        virtual std::vector<IConfigurationSource::SPtr> &getSources() = 0;

        virtual const std::vector<IConfigurationSource::SPtr> &getSources() const = 0;

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

        IConfigurationBuilder &addEnvironmentVariables() { return addEnvironmentVariables("", {}); }

        IConfigurationBuilder &addEnvironmentVariables(std::string prefix)
        {
            return addEnvironmentVariables(std::move(prefix), {});
        }

        IConfigurationBuilder &addEnvironmentVariables(std::string prefix, SettingParserConfig config)
        {
            return add(EnvironmentVarsConfigurationSource::create(std::move(prefix), std::move(config)));
        }

        IConfigurationBuilder &addCommandLine(int argc, char *const *const argv)
        {
            return addCommandLine(argc, argv, {});
        }

        IConfigurationBuilder &addCommandLine(std::vector<std::string_view> args)
        {
            return addCommandLine(std::move(args), {});
        }

        IConfigurationBuilder &addCommandLine(int argc, char *const *const argv, SettingParserConfig config)
        {
            return add(CommandLineConfigurationSource::create(argc, argv, std::move(config)));
        }

        IConfigurationBuilder &addCommandLine(std::vector<std::string_view> args, SettingParserConfig config)
        {
            return add(CommandLineConfigurationSource::create(std::move(args), std::move(config)));
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