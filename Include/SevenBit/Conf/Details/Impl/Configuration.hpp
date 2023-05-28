#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Configuration.hpp"
#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf
{

    INLINE Configuration::Configuration(std::vector<IConfigurationSource::SPtr> sources) : _sources(std::move(sources))
    {
        reload();
    }

    INLINE const JsonObject &Configuration::root() const { return _configuration; }

    INLINE JsonObject &Configuration::root() { return _configuration; }

    INLINE const JsonValue &Configuration::at(const std::string &key) const { return _configuration.at(key); }

    INLINE JsonValue &Configuration::at(const std::string &key) { return _configuration.at(key); }

    INLINE const JsonValue *Configuration::find(std::string_view key) const
    {
        return JsonObjectExt::find(_configuration, key);
    }

    INLINE JsonValue *Configuration::find(std::string_view key) { return JsonObjectExt::find(_configuration, key); }

    INLINE const JsonValue *Configuration::findInner(std::string_view key) const
    {
        return JsonObjectExt::findInner(_configuration, key);
    }

    INLINE JsonValue *Configuration::findInner(std::string_view key)
    {
        return JsonObjectExt::findInner(_configuration, key);
    }

    INLINE const JsonValue *Configuration::findInner(const std::vector<std::string_view> &key) const
    {
        return JsonObjectExt::findInner(_configuration, key);
    }

    INLINE JsonValue *Configuration::findInner(const std::vector<std::string_view> &key)
    {
        return JsonObjectExt::findInner(_configuration, key);
    }

    INLINE JsonValue &Configuration::atInner(std::string_view key)
    {
        if (auto value = findInner(key))
        {
            return *value;
        }
        return throwNullPointnerException(key);
    }

    INLINE const JsonValue &Configuration::atInner(std::string_view key) const
    {
        if (auto value = findInner(key))
        {
            return *value;
        }
        return throwNullPointnerException(key);
    }

    INLINE JsonValue &Configuration::atInner(const std::vector<std::string_view> &key)
    {
        if (auto value = findInner(key))
        {
            return *value;
        }
        return throwNullPointnerException(key);
    }

    INLINE const JsonValue &Configuration::atInner(const std::vector<std::string_view> &key) const
    {
        if (auto value = findInner(key))
        {
            return *value;
        }
        return throwNullPointnerException(key);
    }

    INLINE JsonValue &Configuration::operator[](std::string_view key) { return atInner(key); };

    INLINE const JsonValue &Configuration::operator[](std::string_view key) const { return atInner(key); };

    INLINE JsonValue &Configuration::operator[](const std::vector<std::string_view> &key) { return atInner(key); };

    INLINE const JsonValue &Configuration::operator[](const std::vector<std::string_view> &key) const
    {
        return atInner(key);
    };

    INLINE void Configuration::reload()
    {
        _configuration.clear();
        for (auto &source : _sources)
        {
            auto provider = source->build();
            provider->load();
            auto config = provider->getConfiguration();
            JsonObjectExt::deepMerge(_configuration, std::move(config));
        }
    }

    INLINE const std::vector<IConfigurationSource::SPtr> &Configuration::getSources() const { return _sources; }

    INLINE JsonValue &Configuration::throwNullPointnerException(const std::vector<std::string_view> &key) const
    {
        throw NullPointnerException{"Value was not found: " + utils::join(key, ":")};
    }

    INLINE JsonValue &Configuration::throwNullPointnerException(std::string_view key) const
    {
        throw NullPointnerException{"Value was not found: " + std::string{key}};
    }
} // namespace sb::cf