#pragma once

#include <filesystem>
#include <memory>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/IConfigurationProvider.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/KeyPerConfiguration.hpp"
#include "SevenBit/Conf/KeyPerFileConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    INLINE KeyPerFileConfigurationSource::KeyPerFileConfigurationSource(std::filesystem::path directoryPath,
                                                                        bool isOptional)
        : _directoryPath(std::move(directoryPath)), _isOptional(isOptional)
    {
    }

    INLINE KeyPerFileConfigurationSource::KeyPerFileConfigurationSource(std::filesystem::path directoryPath,
                                                                        bool isOptional, std::string ignorePrefix)
        : _directoryPath(std::move(directoryPath)), _isOptional(isOptional), _ignorePrefix(std::move(ignorePrefix))
    {
    }

    INLINE KeyPerFileConfigurationSource::KeyPerFileConfigurationSource(
        std::filesystem::path directoryPath, bool isOptional,
        std::function<bool(const std::filesystem::path &)> ignoreCondition)
        : _directoryPath(std::move(directoryPath)), _isOptional(isOptional),
          _ignoreCondition(std::move(ignoreCondition))
    {
    }

    INLINE const std::filesystem::path &KeyPerFileConfigurationSource::getDirectoryPath() const
    {
        return _directoryPath;
    }

    INLINE bool KeyPerFileConfigurationSource::getIsOptional() const { return _isOptional; }

    INLINE const std::function<bool(const std::filesystem::path &)> &KeyPerFileConfigurationSource::getIgnoreCondition()
        const
    {
        return _ignoreCondition;
    }

    INLINE const std::string &KeyPerFileConfigurationSource::getIgnorePrefix() const { return _ignorePrefix; }

    INLINE IConfigurationProvider::Ptr KeyPerFileConfigurationSource::build() const
    {
        return std::make_unique<KeyPerFileConfigurationProvider>(*this);
    }

    INLINE KeyPerFileConfigurationProvider::KeyPerFileConfigurationProvider(KeyPerFileConfigurationSource source)
        : _source(std::move(source))
    {
    }

    INLINE void KeyPerFileConfigurationProvider::load()
    {
        _configuration.clear();
        KeyPerConfigurationSource sources{};
        auto &directoryPath = _source.getDirectoryPath();
        if (!std::filesystem::exists(directoryPath) && _source.getIsOptional())
        {
            return;
        }
        for (auto const &entry : std::filesystem::directory_iterator{directoryPath})
        {
            const auto &filePath = entry.path();
            if (!entry.is_regular_file() || canIgnore(filePath))
            {
                continue;
            }
            auto extension = filePath.extension();
            if (extension == ".json")
            {
                sources.add(filePath.stem(), std::make_unique<JsonFileConfigurationSource>(filePath));
            }
        }
        auto provider = sources.build();
        provider->load();
        _configuration = provider->get();
    }

    INLINE const JsonObject &KeyPerFileConfigurationProvider::get() const { return _configuration; }

    INLINE bool KeyPerFileConfigurationProvider::canIgnore(std::filesystem::path filePath) const
    {
        auto &ignorePrefix = _source.getIgnorePrefix();
        if (!ignorePrefix.empty() && utils::startsWith(filePath.filename().generic_string(), ignorePrefix))
        {
            return true;
        }
        auto &ignoreConfition = _source.getIgnoreCondition();
        if (!!ignoreConfition && ignoreConfition(filePath))
        {
            return true;
        }
        return false;
    }

} // namespace sb::cf