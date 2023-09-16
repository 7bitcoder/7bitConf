#pragma once

#include <memory>

#include "SevenBit/Conf/ConfigurationProviderBase.hpp"
#include "SevenBit/Conf/Details/JsonExt.hpp"

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
} // namespace sb::cf
