#pragma once

#include <memory>

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf
{
    INLINE const JsonObject &ConfigurationProviderBase::getConfiguration() const { return _configuration; }

    INLINE JsonObject &ConfigurationProviderBase::getConfiguration() { return _configuration; }

    INLINE void ConfigurationProviderBase::clear() { _configuration.clear(); }

    INLINE void ConfigurationProviderBase::set(JsonObject configuration) { _configuration = std::move(configuration); }

    INLINE void ConfigurationProviderBase::update(JsonObject configuration)
    {
        details::JsonExt::deepMerge(_configuration, std::move(configuration));
    }

    INLINE void ConfigurationProviderBase::update(const std::vector<std::string_view> &keys, JsonValue value)
    {
        details::JsonExt::updateWith(_configuration, keys, std::move(value));
    }

    INLINE void ConfigurationProviderBase::throwNullSourceException() const
    {
        throw NullPointerException{"Source cannot be null"};
    }

    INLINE void ConfigurationProviderBase::throwNullProviderException() const
    {
        throw NullPointerException{"Provider cannot be null"};
    }
} // namespace sb::cf
