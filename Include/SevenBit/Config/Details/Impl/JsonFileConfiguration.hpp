#pragma once

#include <filesystem>
#include <memory>

#include "SevenBit/Config/Exceptions.hpp"
#include "SevenBit/Config/JsonFileConfiguration.hpp"
#include <tao/json/from_file.hpp>

namespace sb::cf
{
    INLINE JsonFileConfigurationSource::JsonFileConfigurationSource(std::filesystem::path filePath, bool isOptional)
        : _filePath(std::move(filePath)), _isOptional(isOptional)
    {
    }

    INLINE JsonFileConfigurationSource::SPtr JsonFileConfigurationSource::create(std::filesystem::path filePath,
                                                                                 bool isOptional)
    {
        return JsonFileConfigurationSource::SPtr{new JsonFileConfigurationSource{std::move(filePath), isOptional}};
    }

    INLINE const std::filesystem::path &JsonFileConfigurationSource::getFilePath() const { return _filePath; }

    INLINE bool JsonFileConfigurationSource::getIsOptional() const { return _isOptional; }

    INLINE IConfigurationProvider::Ptr JsonFileConfigurationSource::build(IConfigurationBuilder &builder)
    {
        return std::make_unique<JsonFileConfigurationProvider>(shared_from_this());
    }

    INLINE JsonFileConfigurationProvider::JsonFileConfigurationProvider(JsonFileConfigurationSource::SPtr source)
        : _source(std::move(source))
    {
    }

    INLINE void JsonFileConfigurationProvider::load() { set(getJsonFromFile()); }

    INLINE JsonObject JsonFileConfigurationProvider::getJsonFromFile()
    {
        if (!std::filesystem::exists(_source->getFilePath()))
        {
            if (_source->getIsOptional())
            {
                return JsonObject{};
            }
            throw ConfigFileNotFoundException(_source->getFilePath());
        }
        auto json = tao::json::basic_from_file<JsonTraits>(_source->getFilePath());
        if (!json.is_object())
        {
            throw BadConfigFileException(_source->getFilePath(), "file does not contain json object");
        }
        return json.get_object();
    }
} // namespace sb::cf