#pragma once

#include <string>
#include <tao/json/to_string.hpp>

#include "SevenBit/Conf/Configuration.hpp"
#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

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
        return details::JsonExt::find(root(), key);
    }

    INLINE JsonValue *Configuration::find(std::string_view key) { return details::JsonExt::find(root(), key); }

    INLINE const JsonValue *Configuration::deepFind(std::string_view key) const
    {
        return details::JsonExt::deepFind(rootAsObject(), key);
    }

    INLINE JsonValue *Configuration::deepFind(std::string_view key)
    {
        return details::JsonExt::deepFind(rootAsObject(), key);
    }

    INLINE const JsonValue *Configuration::deepFind(const std::vector<std::string_view> &key) const
    {
        return details::JsonExt::deepFind(rootAsObject(), key);
    }

    INLINE JsonValue *Configuration::deepFind(const std::vector<std::string_view> &key)
    {
        return details::JsonExt::deepFind(rootAsObject(), key);
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
        return details::JsonExt::deepGetOrOverride(rootAsObject(), key);
    }

    INLINE const JsonValue &Configuration::operator[](std::string_view key) const { return deepAt(key); }

    INLINE JsonValue &Configuration::operator[](const std::vector<std::string_view> &key)
    {
        return details::JsonExt::deepGetOrOverride(rootAsObject(), key);
    }

    INLINE const JsonValue &Configuration::operator[](const std::vector<std::string_view> &key) const
    {
        return deepAt(key);
    }

    INLINE void Configuration::reload()
    {
        auto &configRoot = rootAsObject();
        configRoot.clear();
        for (auto &provider : _providers)
        {
            details::utils::assertPtr(provider);
            provider->load();
            details::JsonExt::deepMerge(configRoot, std::move(provider->getConfiguration()));
        }
    }

    INLINE const std::vector<IConfigurationProvider::Ptr> &Configuration::getProviders() const { return _providers; }

    INLINE std::vector<IConfigurationProvider::Ptr> &Configuration::getProviders() { return _providers; }

    INLINE JsonValue &Configuration::throwNotFoundException(const std::vector<std::string_view> &key) const
    {
        throw ValueNotFoundException{"Value was not found for key: " + details::utils::joinViews(key, ":")};
    }

    INLINE JsonValue &Configuration::throwNotFoundException(std::string_view key) const
    {
        throw ValueNotFoundException{"Value was not found for key: " + std::string{key}};
    }
} // namespace sb::cf
