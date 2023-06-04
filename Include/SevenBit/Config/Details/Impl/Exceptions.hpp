#pragma once

#include "SevenBit/Config/Exceptions.hpp"

namespace sb::cf
{
    INLINE ConfigException::ConfigException(const std::string &error) : std::runtime_error(error) {}

    INLINE NullPointnerException::NullPointnerException(const std::string &why) : ConfigException{why} {}

    INLINE ConfigFileNotFoundException::ConfigFileNotFoundException(const std::filesystem::path &filePath)
        : ConfigException("Configuration file '" + filePath.generic_string() + "' does not exist")
    {
    }

    INLINE
    BadConfigFileException::BadConfigFileException(const std::filesystem::path &filePath, const std::string &why)
        : ConfigException("Cannot read configuration file '" + filePath.generic_string() + "' beacuse: " + why)
    {
    }

    INLINE BadStreamException::BadStreamException(const std::string &why) : ConfigException{why} {}

    INLINE SettingParserException::SettingParserException(const std::string &why) : ConfigException{why} {}
} // namespace sb::cf