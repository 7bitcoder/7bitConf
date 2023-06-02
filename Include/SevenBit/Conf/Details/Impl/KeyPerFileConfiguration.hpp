#pragma once

#include <algorithm>
#include <filesystem>
#include <memory>

#include "SevenBit/Conf/ChainedConfiguration.hpp"
#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/IConfigurationProvider.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/KeyPerFileConfiguration.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"
#include "SevenBit/Conf/MapConfiguration.hpp"

namespace sb::cf
{
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

    INLINE KeyPerFileConfigurationSource::SPtr KeyPerFileConfigurationSource::create(
        std::filesystem::path directoryPath, bool isOptional, std::string ignorePrefix)
    {
        return KeyPerFileConfigurationSource::SPtr{
            new KeyPerFileConfigurationSource{std::move(directoryPath), isOptional, std::move(ignorePrefix)}};
    }

    INLINE KeyPerFileConfigurationSource::SPtr KeyPerFileConfigurationSource::create(
        std::filesystem::path directoryPath, bool isOptional,
        std::function<bool(const std::filesystem::path &)> ignoreCondition)
    {
        return KeyPerFileConfigurationSource::SPtr{
            new KeyPerFileConfigurationSource{std::move(directoryPath), isOptional, std::move(ignoreCondition)}};
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

    INLINE IConfigurationProvider::Ptr KeyPerFileConfigurationSource::build(IConfigurationBuilder &builder)
    {
        auto sources = ChainedConfigurationSource::create();

        auto &directoryPath = getDirectoryPath();
        if (!std::filesystem::exists(directoryPath) && getIsOptional())
        {
            return sources->build(builder);
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
                auto fileSource = JsonFileConfigurationSource::create(filePath);
                auto mapSource = MapConfigurationSource::create(std::move(fileSource), [filePath](JsonObject config) -> JsonObject {
                    return JsonObject{{filePath.stem().generic_string(), std::move(config)}};
                });
                sources->add(mapSource);
            }
        }
        return sources->build(builder);
    }

    INLINE bool KeyPerFileConfigurationSource::canIgnore(std::filesystem::path filePath) const
    {
        auto &ignorePrefix = getIgnorePrefix();
        if (!ignorePrefix.empty() && utils::startsWith(filePath.filename().generic_string(), ignorePrefix))
        {
            return true;
        }
        auto &ignoreConfition = getIgnoreCondition();
        if (!!ignoreConfition && ignoreConfition(filePath))
        {
            return true;
        }
        return false;
    }

} // namespace sb::cf