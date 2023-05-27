#pragma once

#include <filesystem>
#include <functional>
#include <memory>

#include "SevenBit/Conf/IConfigurationProvider.hpp"
#include "SevenBit/Conf/KeyPerConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    EXPORT class KeyPerFileConfigurationSource : public IConfigurationSource
    {
      private:
        std::filesystem::path _directoryPath;
        std::function<bool(const std::filesystem::path &)> _ignoreCondition;
        std::string _ignorePrefix;
        bool _isOptional;

      public:
        KeyPerFileConfigurationSource(std::filesystem::path directoryPath, bool isOptional = false);

        KeyPerFileConfigurationSource(std::filesystem::path directoryPath, bool isOptional, std::string ignorePrefix);

        KeyPerFileConfigurationSource(std::filesystem::path directoryPath, bool isOptional,
                                      std::function<bool(const std::filesystem::path &)> ignoreCondition);

        const std::filesystem::path &getDirectoryPath() const;

        const std::function<bool(const std::filesystem::path &)> &getIgnoreCondition() const;

        const std::string &getIgnorePrefix() const;

        bool getIsOptional() const;

        IConfigurationProvider::Ptr build() const override;
    };

    EXPORT class KeyPerFileConfigurationProvider : public IConfigurationProvider
    {
      private:
        KeyPerFileConfigurationSource _source;

        JsonObject _configuration;

      public:
        KeyPerFileConfigurationProvider(KeyPerFileConfigurationSource source);

        void load() override;

        const JsonObject &get() const override;

      private:
        bool canIgnore(std::filesystem::path filePath) const;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/KeyPerFileConfiguration.hpp"
#endif