#pragma once

#include <filesystem>
#include <memory>

#include "SevenBit/Config/LibraryConfig.hpp"

#include "SevenBit/Config/ConfigurationProviderBase.hpp"
#include "SevenBit/Config/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class JsonFileConfigurationSource : public IConfigurationSource,
                                               public std::enable_shared_from_this<JsonFileConfigurationSource>
    {
      private:
        std::filesystem::path _filePath;
        bool _isOptional;

        JsonFileConfigurationSource(std::filesystem::path filePath, bool isOptional);

      public:
        using Ptr = std::unique_ptr<JsonFileConfigurationSource>;
        using SPtr = std::shared_ptr<JsonFileConfigurationSource>;

        static SPtr create(std::filesystem::path filePath, bool isOptional = false);

        const std::filesystem::path &getFilePath() const;

        bool getIsOptional() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;
    };

    EXPORT class JsonFileConfigurationProvider : public ConfigurationProviderBase
    {
      private:
        JsonFileConfigurationSource::SPtr _source;

      public:
        JsonFileConfigurationProvider(JsonFileConfigurationSource::SPtr source);

        void load() override;

      private:
        JsonObject getJsonFromFile();
    };
} // namespace sb::cf

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/JsonFileConfiguration.hpp"
#endif