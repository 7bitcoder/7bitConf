#pragma once

#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/StringUtils.hpp"
#include "SevenBit/Conf/Sources/ChainedConfiguration.hpp"
#include "SevenBit/Conf/Sources/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/Sources/KeyPerFileConfiguration.hpp"
#include "SevenBit/Conf/Sources/MapConfiguration.hpp"

namespace sb::cf
{
    INLINE KeyPerFileConfigurationSource::KeyPerFileConfigurationSource(std::filesystem::path directoryPath,
                                                                        bool isOptional, std::string ignorePrefix)
        : _directoryPath(std::move(directoryPath)), _ignorePrefix(std::move(ignorePrefix)), _isOptional(isOptional)
    {
    }

    INLINE KeyPerFileConfigurationSource::KeyPerFileConfigurationSource(
        std::filesystem::path directoryPath, bool isOptional,
        std::function<bool(const std::filesystem::path &)> ignoreCondition)
        : _directoryPath(std::move(directoryPath)), _ignoreCondition(std::move(ignoreCondition)),
          _isOptional(isOptional)
    {
    }

    INLINE KeyPerFileConfigurationSource::Ptr KeyPerFileConfigurationSource::create(std::filesystem::path directoryPath,
                                                                                    bool isOptional,
                                                                                    std::string ignorePrefix)
    {
        return std::make_unique<KeyPerFileConfigurationSource>(std::move(directoryPath), isOptional,
                                                               std::move(ignorePrefix));
    }

    INLINE KeyPerFileConfigurationSource::Ptr KeyPerFileConfigurationSource::create(
        std::filesystem::path directoryPath, bool isOptional,
        std::function<bool(const std::filesystem::path &)> ignoreCondition)
    {
        return std::make_unique<KeyPerFileConfigurationSource>(std::move(directoryPath), isOptional,
                                                               std::move(ignoreCondition));
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
        ChainedConfigurationSource sources;

        auto &directoryPath = getDirectoryPath();
        if (!std::filesystem::exists(directoryPath) && getIsOptional())
        {
            return sources.build(builder);
        }
        for (auto const &entry : std::filesystem::directory_iterator{directoryPath})
        {
            if (auto source = tryGetMappedFileSource(entry))
            {
                sources.add(source);
            }
        }
        return sources.build(builder);
    }

    INLINE IConfigurationSource::SPtr KeyPerFileConfigurationSource::tryGetMappedFileSource(
        const std::filesystem::directory_entry &entry) const
    {
        if (const auto &filePath = entry.path(); entry.is_regular_file())
        {
            if (auto fileSource = tryGetFileSource(filePath))
            {
                auto mapFcn = [name = filePath.stem().generic_string()](const JsonObject &config) -> JsonObject {
                    auto res = JsonObject{};
                    const auto keys = details::StringUtils::split(name, "__");
                    auto &optimized = const_cast<JsonObject &>(config);
                    details::JsonExt::updateWith(res, keys, std::move(optimized));
                    return res;
                };
                return MapConfigurationSource::create(std::move(fileSource), std::move(mapFcn));
            }
        }
        return nullptr;
    }

    INLINE IConfigurationSource::SPtr KeyPerFileConfigurationSource::tryGetFileSource(
        const std::filesystem::path &filePath) const
    {
        if (!canIgnore(filePath))
        {
            const auto &extension = filePath.extension();
            if (extension == ".json")
            {
                return JsonFileConfigurationSource::create(filePath);
            }
        }
        return nullptr;
    }

    INLINE bool KeyPerFileConfigurationSource::canIgnore(const std::filesystem::path &filePath) const
    {
        auto &ignorePrefix = getIgnorePrefix();
        if (!ignorePrefix.empty() &&
            details::StringUtils::startsWith(filePath.filename().generic_string(), ignorePrefix))
        {
            return true;
        }
        auto &ignoreCondition = getIgnoreCondition();
        if (ignoreCondition && ignoreCondition(filePath))
        {
            return true;
        }
        return false;
    }
} // namespace sb::cf
