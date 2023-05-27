#pragma once

#include <filesystem>
#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class JsonFileConfigurationSource : public IConfigurationSource
    {
      private:
        std::filesystem::path _filePath;
        bool _isOptional;

      public:
        JsonFileConfigurationSource(std::filesystem::path filePath, bool isOptional = false);

        const std::filesystem::path &getFilePath();

        bool getIsOptional();

        IConfigurationProvider::Ptr build() const override;
    };

    EXPORT class JsonFileConfigurationProvider : public IConfigurationProvider
    {
      private:
        JsonFileConfigurationSource _source;

        JsonObject _configuration;

      public:
        JsonFileConfigurationProvider(JsonFileConfigurationSource source);

        void load() override;

        const JsonObject &get() const override;

      private:
        JsonObject getJsonFromFile();
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonFileConfiguration.hpp"
#endif