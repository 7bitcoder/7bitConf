#pragma once

#include "SevenBit/Config/Configuration.hpp"
#include "SevenBit/Config/Details/JsonObjectExt.hpp"
#include "SevenBit/Config/Details/Utils.hpp"
#include "SevenBit/Config/Exceptions.hpp"
#include "SevenBit/Config/Json.hpp"
#include <tao/json/to_string.hpp>

namespace sb::cf
{

    INLINE Configuration::Configuration(std::vector<IConfigurationProvider::Ptr> providers)
        : _providers(std::move(providers))
    {
        reload();
    }

    INLINE std::string Configuration::toString(std::size_t indent, std::string newLineMark) const
    {
        return json::to_string(_configuration, indent, std::move(newLineMark));
    }

    INLINE const JsonValue &Configuration::root() const { return _configuration; }

    INLINE JsonValue &Configuration::root() { return _configuration; }

    INLINE const JsonObject &Configuration::rootAsObject() const { return _configuration.get_object(); }

    INLINE JsonObject &Configuration::rootAsObject() { return _configuration.get_object(); }

    INLINE const JsonValue &Configuration::at(const std::string &key) const { return rootAsObject().at(key); }

    INLINE JsonValue &Configuration::at(const std::string &key) { return rootAsObject().at(key); }

    INLINE const JsonValue *Configuration::find(std::string_view key) const
    {
        return details::JsonObjectExt::find(root(), key);
    }

    INLINE JsonValue *Configuration::find(std::string_view key) { return details::JsonObjectExt::find(root(), key); }

    INLINE const JsonValue *Configuration::findInner(std::string_view key) const
    {
        return details::JsonObjectExt::findInner(rootAsObject(), key);
    }

    INLINE JsonValue *Configuration::findInner(std::string_view key)
    {
        return details::JsonObjectExt::findInner(rootAsObject(), key);
    }

    INLINE const JsonValue *Configuration::findInner(const std::vector<std::string_view> &key) const
    {
        return details::JsonObjectExt::findInner(rootAsObject(), key);
    }

    INLINE JsonValue *Configuration::findInner(const std::vector<std::string_view> &key)
    {
        return details::JsonObjectExt::findInner(rootAsObject(), key);
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

    INLINE JsonValue &Configuration::operator[](std::string_view key)
    {
        return details::JsonObjectExt::getOrCreateInner(rootAsObject(), key);
    };

    INLINE const JsonValue &Configuration::operator[](std::string_view key) const { return atInner(key); };

    INLINE JsonValue &Configuration::operator[](const std::vector<std::string_view> &key)
    {
        return details::JsonObjectExt::getOrCreateInner(rootAsObject(), key);
    };

    INLINE const JsonValue &Configuration::operator[](const std::vector<std::string_view> &key) const
    {
        return atInner(key);
    };

    INLINE void Configuration::reload()
    {
        auto &configRoot = rootAsObject();
        configRoot.clear();
        for (auto &provider : _providers)
        {
            provider->load();
            details::JsonObjectExt::deepMerge(configRoot, std::move(provider->getConfiguration()));
        }
    }

    INLINE const std::vector<IConfigurationProvider::Ptr> &Configuration::getProviders() const { return _providers; }

    INLINE JsonValue &Configuration::throwNullPointnerException(const std::vector<std::string_view> &key) const
    {
        throw NullPointnerException{"Value was not found: " + utils::join(key, ":")};
    }

    INLINE JsonValue &Configuration::throwNullPointnerException(std::string_view key) const
    {
        throw NullPointnerException{"Value was not found: " + std::string{key}};
    }
} // namespace sb::cf