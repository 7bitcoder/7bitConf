#pragma once

#include <filesystem>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/AppSettingsConfiguration.hpp"
#include "SevenBit/Conf/CommandLineConfiguration.hpp"
#include "SevenBit/Conf/EnvironmentVarsConfiguration.hpp"
#include "SevenBit/Conf/IConfigurationRoot.hpp"
#include "SevenBit/Conf/JsonConfiguration.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/JsonStreamConfiguration.hpp"
#include "SevenBit/Conf/KeyPerFileConfiguration.hpp"

namespace sb::cf
{
    struct IConfigurationBuilder
    {
        using Ptr = std::unique_ptr<IConfigurationBuilder>;

        virtual IConfigurationRoot::Ptr build() = 0;

        virtual IConfigurationBuilder &add(IConfigurationSource::Ptr) = 0;

        IConfigurationBuilder &addJsonFile(std::filesystem::path filePath, bool isOptional = false)
        {
            return add(std::make_unique<JsonFileConfigurationSource>(filePath, isOptional));
        }

        IConfigurationBuilder &addJsonStream(std::istringstream &stream)
        {
            return add(std::make_unique<JsonStreamConfigurationSource>(stream));
        }

        IConfigurationBuilder &addAppSettings(std::string envName = "")
        {
            return add(std::make_unique<AppSettingsConfigurationSource>(std::move(envName)));
        }

        IConfigurationBuilder &addEnvironmentVariables(std::string prefix = "")
        {
            return add(std::make_unique<EnvironmentVarsConfigurationSource>(std::move(prefix)));
        }

        IConfigurationBuilder &addCommandLine(int argc, char **argv)
        {
            return add(std::make_unique<CommandLineConfigurationSource>(argc, argv));
        }

        IConfigurationBuilder &addJson(JsonObject object)
        {
            return add(std::make_unique<JsonConfigurationSource>(std::move(object)));
        }

        IConfigurationBuilder &addKeyPerFile(std::filesystem::path directoryPath)
        {
            return add(std::make_unique<KeyPerFileConfigurationSource>(std::move(directoryPath)));
        }

        virtual const std::vector<IConfigurationSource::SPtr> &getSources() const = 0;

        virtual ~IConfigurationBuilder() = default;
    };
} // namespace sb::cf