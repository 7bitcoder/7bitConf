#pragma once

#include <memory>

#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Sources/ConfigurationProviderBase.hpp"

namespace sb::cf
{
    INLINE const JsonObject &ConfigurationProviderBase::getConfiguration() const { return _configuration; }

    INLINE JsonObject &ConfigurationProviderBase::getConfiguration() { return _configuration; }

    INLINE void ConfigurationProviderBase::clear() { _configuration.clear(); }

    INLINE void ConfigurationProviderBase::set(const JsonObject &configuration) { _configuration = configuration; }

    INLINE void ConfigurationProviderBase::set(JsonObject &&configuration)
    {
        _configuration = std::move(configuration);
    }

    INLINE void ConfigurationProviderBase::update(const JsonObject &configuration)
    {
        details::JsonExt::deepMerge(_configuration, configuration);
    }

    INLINE void ConfigurationProviderBase::update(JsonObject &&configuration)
    {
        details::JsonExt::deepMerge(_configuration, std::move(configuration));
    }

    INLINE void ConfigurationProviderBase::updateWith(std::string_view key, const JsonValue &value)
    {
        details::JsonExt::updateWith(_configuration, key, value);
    }

    INLINE void ConfigurationProviderBase::updateWith(std::string_view key, JsonValue &&value)
    {
        details::JsonExt::updateWith(_configuration, key, std::move(value));
    }

    INLINE void ConfigurationProviderBase::updateWith(const std::vector<std::string_view> &keys, const JsonValue &value)
    {
        details::JsonExt::updateWith(_configuration, keys, value);
    }

    INLINE void ConfigurationProviderBase::updateWith(const std::vector<std::string_view> &keys, JsonValue &&value)
    {
        details::JsonExt::updateWith(_configuration, keys, std::move(value));
    }
} // namespace sb::cf
