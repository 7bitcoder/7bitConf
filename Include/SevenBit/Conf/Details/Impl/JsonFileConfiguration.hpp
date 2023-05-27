#pragma once

#include <filesystem>
#include <memory>

#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/JsonFileConfiguration.hpp"

namespace sb::cf
{
    INLINE JsonFileConfigurationProvider::JsonFileConfigurationProvider(JsonFileConfigurationSource source)
        : _source(std::move(source))
    {
    }

    INLINE void JsonFileConfigurationProvider::load() { _configuration = getJsonFromFile(); }

    INLINE const JsonObject &JsonFileConfigurationProvider::get() const { return _configuration; }

    INLINE JsonObject JsonFileConfigurationProvider::getJsonFromFile()
    {
        bool exists = std::filesystem::exists(_source.getFilePath());
        if (!exists)
        {
            return _source.getIsOptional() ? JsonObject{} : throw ConfigFileNotFoundException(_source.getFilePath());
        }
        auto json = tao::json::basic_from_file<JsonTraits>(_source.getFilePath());
        if (!json.is_object())
        {
            throw BadConfigFileException(_source.getFilePath(), "file does not contain json object");
        }
        return json.get_object();
    }

    INLINE JsonFileConfigurationSource::JsonFileConfigurationSource(std::filesystem::path filePath, bool isOptional)
        : _filePath(std::move(filePath)), _isOptional(isOptional)
    {
    }

    INLINE const std::filesystem::path &JsonFileConfigurationSource::getFilePath() { return _filePath; }

    INLINE bool JsonFileConfigurationSource::getIsOptional() { return _isOptional; }

    INLINE IConfigurationProvider::Ptr JsonFileConfigurationSource::build() const
    {
        return std::make_unique<JsonFileConfigurationProvider>(*this);
    }
} // namespace sb::cf