#pragma once

#include <exception>
#include <filesystem>
#include <stdexcept>
#include <string>

#include "SevenBit/Config/LibraryConfig.hpp"

namespace sb::cf
{
    /**
     * @brief Base class for all config exceptions
     */
    struct EXPORT ConfigException : public std::runtime_error
    {
        ConfigException(const std::string &error);
    };

    struct EXPORT NullPointnerException : public ConfigException
    {
        NullPointnerException(const std::string &why);
    };

    struct EXPORT ConfigFileNotFoundException : public ConfigException
    {
        ConfigFileNotFoundException(const std::filesystem::path &filePath);
    };

    struct EXPORT BadConfigFileException : public ConfigException
    {
        BadConfigFileException(const std::filesystem::path &filePath, const std::string &why);
    };

    struct EXPORT BadStreamException : public ConfigException
    {
        BadStreamException(const std::string &why);
    };

    struct EXPORT SettingParserException : public ConfigException
    {
        SettingParserException(const std::string &why);
    };
} // namespace sb::cf

#ifdef _7BIT_CONFIG_ADD_IMPL
#include "SevenBit/Config/Details/Impl/Exceptions.hpp"
#endif