#pragma once

#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf
{
    INLINE ConfigException::ConfigException(const std::string &error) : std::runtime_error(error) {}

    INLINE NullPointerException::NullPointerException(const std::string &why) : ConfigException{why} {}

    INLINE ValueNotFoundException::ValueNotFoundException(const std::string &why) : ConfigException{why} {}

    INLINE ConfigFileNotFoundException::ConfigFileNotFoundException(const std::filesystem::path &filePath)
        : ConfigException("Configuration file '" + filePath.generic_string() + "' does not exist")
    {
    }

    INLINE
    BadConfigFileException::BadConfigFileException(const std::filesystem::path &filePath, const std::string &why)
        : ConfigException("Cannot read configuration file '" + filePath.generic_string() + "' because: " + why)
    {
    }

    INLINE BadStreamException::BadStreamException(const std::string &why) : ConfigException{why} {}

    INLINE SettingParserException::SettingParserException(const std::string &why) : ConfigException{why} {}
} // namespace sb::cf
