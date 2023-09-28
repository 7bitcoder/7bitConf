#pragma once

#include <exception>
#include <filesystem>
#include <stdexcept>
#include <string>

#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    /**
     * @brief Base class for all config exceptions
     */
    struct EXPORT ConfigException : public std::runtime_error
    {
        explicit ConfigException(const std::string &error);
    };

    struct EXPORT NullPointerException : public ConfigException
    {
        explicit NullPointerException(const std::string &why);
    };

    struct EXPORT ValueNotFoundException : public ConfigException
    {
        explicit ValueNotFoundException(const std::string &why);
    };

    struct EXPORT ConfigFileNotFoundException : public ConfigException
    {
        explicit ConfigFileNotFoundException(const std::filesystem::path &filePath);
    };

    struct EXPORT BadConfigFileException : public ConfigException
    {
        BadConfigFileException(const std::filesystem::path &filePath, const std::string &why);
    };

    struct EXPORT BadStreamException : public ConfigException
    {
        explicit BadStreamException(const std::string &why);
    };

    struct EXPORT SettingParserException : public ConfigException
    {
        explicit SettingParserException(const std::string &why);
    };
} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/Exceptions.hpp"
#endif
