#pragma once

#include <filesystem>
#include <tao/json/from_file.hpp>

#include "SevenBit/Conf/Details/Require.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Sources/JsonFileConfiguration.hpp"

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
        details::Require::notNull(_source);
    }

    INLINE void JsonFileConfigurationProvider::load() { set(getJsonFromFile()); }

    INLINE JsonObject JsonFileConfigurationProvider::getJsonFromFile()
    {
        auto &filePath = _source->getFilePath();
        if (!std::filesystem::exists(filePath))
        {
            if (_source->getIsOptional())
            {
                return JsonObject{};
            }
            throw ConfigFileNotFoundException(filePath);
        }
        auto json = tao::json::basic_from_file<JsonTraits>(filePath);
        if (!json.is_object())
        {
            throw BadConfigFileException(filePath, "file does not contain json object");
        }
        return json.get_object();
    }
} // namespace sb::cf
