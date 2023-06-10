#pragma once

#include <filesystem>
#include <memory>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    class EXPORT JsonFileConfigurationSource : public IConfigurationSource,
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

    class EXPORT JsonFileConfigurationProvider : public ConfigurationProviderBase
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

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonFileConfiguration.hpp"
#endif