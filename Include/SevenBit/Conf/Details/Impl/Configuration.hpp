#pragma once

#include <string>
#include <tao/json/to_string.hpp>

#include "SevenBit/Conf/Details/Configuration.hpp"
#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/StringUtils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf::details
{

    INLINE Configuration::Configuration(std::vector<IConfigurationProvider::Ptr> providers)
        : _providers(std::move(providers))
    {
        buildConfig(true);
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

    INLINE const JsonValue *Configuration::find(std::string_view key) const { return JsonExt::find(root(), key); }

    INLINE JsonValue *Configuration::find(std::string_view key) { return JsonExt::find(root(), key); }

    INLINE const JsonValue *Configuration::deepFind(std::string_view key) const
    {
        return JsonExt::deepFind(rootAsObject(), key);
    }

    INLINE JsonValue *Configuration::deepFind(std::string_view key) { return JsonExt::deepFind(rootAsObject(), key); }

    INLINE const JsonValue *Configuration::deepFind(const std::vector<std::string_view> &key) const
    {
        return JsonExt::deepFind(rootAsObject(), key);
    }

    INLINE JsonValue *Configuration::deepFind(const std::vector<std::string_view> &key)
    {
        return JsonExt::deepFind(rootAsObject(), key);
    }

    INLINE JsonValue &Configuration::deepAt(std::string_view key)
    {
        auto value = deepFind(key);
        return value ? *value : throwNotFoundException(key);
    }

    INLINE const JsonValue &Configuration::deepAt(std::string_view key) const
    {
        auto value = deepFind(key);
        return value ? *value : throwNotFoundException(key);
    }

    INLINE JsonValue &Configuration::deepAt(const std::vector<std::string_view> &key)
    {
        auto value = deepFind(key);
        return value ? *value : throwNotFoundException(key);
    }

    INLINE const JsonValue &Configuration::deepAt(const std::vector<std::string_view> &key) const
    {
        auto value = deepFind(key);
        return value ? *value : throwNotFoundException(key);
    }

    INLINE JsonValue &Configuration::operator[](std::string_view key)
    {
        return JsonExt::deepGetOrOverride(rootAsObject(), key);
    }

    INLINE const JsonValue &Configuration::operator[](std::string_view key) const { return deepAt(key); }

    INLINE JsonValue &Configuration::operator[](const std::vector<std::string_view> &key)
    {
        return JsonExt::deepGetOrOverride(rootAsObject(), key);
    }

    INLINE const JsonValue &Configuration::operator[](const std::vector<std::string_view> &key) const
    {
        return deepAt(key);
    }

    INLINE void Configuration::load()
    {
        for (auto &provider : _providers)
        {
            Require::notNull(provider);
            provider->load();
        }
    }

    INLINE void Configuration::buildConfig(const bool withLoad)
    {
        auto &configRoot = rootAsObject();
        configRoot.clear();
        for (auto &provider : _providers)
        {
            Require::notNull(provider);
            if (withLoad)
            {
                provider->load();
            }
            JsonExt::deepMerge(configRoot, provider->getConfiguration());
        }
    }

    INLINE const std::vector<IConfigurationProvider::Ptr> &Configuration::getProviders() const { return _providers; }

    INLINE std::vector<IConfigurationProvider::Ptr> &Configuration::getProviders() { return _providers; }

    INLINE JsonValue &Configuration::throwNotFoundException(const std::vector<std::string_view> &key)
    {
        throw ValueNotFoundException{"Value was not found for key: " + StringUtils::join(key, ":")};
    }

    INLINE JsonValue &Configuration::throwNotFoundException(std::string_view key)
    {
        throw ValueNotFoundException{"Value was not found for key: " + std::string{key}};
    }
} // namespace sb::cf::details
