#pragma once

#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Sources/ChainedConfiguration.hpp"
#include "SevenBit/Conf/Sources/JsonFileConfiguration.hpp"
#include "SevenBit/Conf/Sources/KeyPerFileConfiguration.hpp"
#include "SevenBit/Conf/Sources/MapConfiguration.hpp"

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
            const auto &filePath = entry.path();
            if (!entry.is_regular_file() || canIgnore(filePath))
            {
                continue;
            }
            auto extension = filePath.extension();
            if (extension == ".json")
            {
                auto fileSource = JsonFileConfigurationSource::create(filePath);
                auto mapSource = MapConfigurationSource::create(
                    std::move(fileSource), [name = filePath.stem().generic_string()](JsonObject config) -> JsonObject {
                        auto res = JsonObject{};
                        details::JsonExt::deepGetOrOverride(res, details::utils::split(name, "__")) = std::move(config);
                        return res;
                    });
                sources.add(mapSource);
            }
        }
        return sources.build(builder);
    }

    INLINE bool KeyPerFileConfigurationSource::canIgnore(const std::filesystem::path &filePath) const
    {
        auto &ignorePrefix = getIgnorePrefix();
        if (!ignorePrefix.empty() && details::utils::startsWith(filePath.filename().generic_string(), ignorePrefix))
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
