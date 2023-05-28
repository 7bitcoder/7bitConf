#pragma once

#include <filesystem>
#include <functional>
#include <memory>

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationProvider.hpp"
#include "SevenBit/Conf/KeyPerConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class KeyPerFileConfigurationSource : public IConfigurationSource,
                                                 public std::enable_shared_from_this<KeyPerFileConfigurationSource>
    {
      private:
        std::filesystem::path _directoryPath;
        std::function<bool(const std::filesystem::path &)> _ignoreCondition;
        std::string _ignorePrefix;
        bool _isOptional;

        KeyPerFileConfigurationSource(std::filesystem::path directoryPath, bool isOptional = false,
                                      std::string ignorePrefix = "");

        KeyPerFileConfigurationSource(std::filesystem::path directoryPath, bool isOptional,
                                      std::function<bool(const std::filesystem::path &)> ignoreCondition);

      public:
        using Ptr = std::unique_ptr<KeyPerFileConfigurationSource>;
        using SPtr = std::shared_ptr<KeyPerFileConfigurationSource>;

        static SPtr create(std::filesystem::path directoryPath, bool isOptional = false, std::string ignorePrefix = "");

        static SPtr create(std::filesystem::path directoryPath, bool isOptional,
                           std::function<bool(const std::filesystem::path &)> ignoreCondition);

        const std::filesystem::path &getDirectoryPath() const;

        const std::function<bool(const std::filesystem::path &)> &getIgnoreCondition() const;

        const std::string &getIgnorePrefix() const;

        bool getIsOptional() const;

        IConfigurationProvider::Ptr build() override;
    };

    EXPORT class KeyPerFileConfigurationProvider : public ConfigurationProviderBase<KeyPerFileConfigurationSource>
    {
      public:
        using ConfigurationProviderBase<KeyPerFileConfigurationSource>::ConfigurationProviderBase;

        void load() override;

      private:
        bool canIgnore(std::filesystem::path filePath) const;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/KeyPerFileConfiguration.hpp"
#endif