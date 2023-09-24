#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <string>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/IConfigurationSource.hpp"

namespace sb::cf
{
    class EXPORT KeyPerFileConfigurationSource : public IConfigurationSource
    {
      private:
        std::filesystem::path _directoryPath;
        std::function<bool(const std::filesystem::path &)> _ignoreCondition;
        std::string _ignorePrefix;
        bool _isOptional;

      public:
        using Ptr = std::unique_ptr<KeyPerFileConfigurationSource>;
        using SPtr = std::shared_ptr<KeyPerFileConfigurationSource>;

        KeyPerFileConfigurationSource(std::filesystem::path directoryPath, bool isOptional = false,
                                      std::string ignorePrefix = "");

        KeyPerFileConfigurationSource(std::filesystem::path directoryPath, bool isOptional,
                                      std::function<bool(const std::filesystem::path &)> ignoreCondition);

        static Ptr create(std::filesystem::path directoryPath, bool isOptional = false, std::string ignorePrefix = "");

        static Ptr create(std::filesystem::path directoryPath, bool isOptional,
                          std::function<bool(const std::filesystem::path &)> ignoreCondition);

        const std::filesystem::path &getDirectoryPath() const;

        const std::function<bool(const std::filesystem::path &)> &getIgnoreCondition() const;

        const std::string &getIgnorePrefix() const;

        bool getIsOptional() const;

        IConfigurationProvider::Ptr build(IConfigurationBuilder &builder) override;

      private:
        bool canIgnore(const std::filesystem::path &filePath) const;
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/KeyPerFileConfiguration.hpp"
#endif