#pragma once

#include <filesystem>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

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

        virtual IConfigurationBuilder &add(IConfigurationSource::SPtr) = 0;

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

        IConfigurationBuilder &addEnvironmentVariables(std::string prefix = "")
        {
            return add(EnvironmentVarsConfigurationSource::create(std::move(prefix)));
        }

        IConfigurationBuilder &addCommandLine(int argc, char **argv)
        {
            return add(CommandLineConfigurationSource::create(argc, argv));
        }

        IConfigurationBuilder &addJson(JsonObject object)
        {
            return add(JsonConfigurationSource::create(std::move(object)));
        }

        IConfigurationBuilder &addKeyPerFile(std::filesystem::path directoryPath)
        {
            return add(KeyPerFileConfigurationSource::create(std::move(directoryPath)));
        }

        virtual const std::vector<IConfigurationSource::SPtr> &getSources() const = 0;

        virtual ~IConfigurationBuilder() = default;
    };
} // namespace sb::cf