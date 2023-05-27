#pragma once

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"

namespace sb::cf
{
    INLINE void JsonObjectExt::deepMerge(JsonObject &json, const JsonObject &override)
    {
        for (auto &[key, value] : override)
        {
            auto it = json.find(key);
            if (it == json.end() || !it->second.is_object() || !value.is_object())
            {
                json[key] = std::move(value);
            }
            else
            {
                deepMerge(it->second.get_object(), value.get_object());
            }
        }
    }

    INLINE JsonValue *JsonObjectExt::find(JsonObject &json, std::string_view key)
    {
        if (auto it = json.find(key); it != json.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    INLINE const JsonValue *JsonObjectExt::find(const JsonObject &json, std::string_view key)
    {
        if (auto it = json.find(key); it != json.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    INLINE JsonValue *JsonObjectExt::findInner(JsonObject &json, std::string_view key)
    {
        return findInner(json, utils::split(key, ":"));
    }

    INLINE const JsonValue *JsonObjectExt::findInner(const JsonObject &json, std::string_view key)
    {
        return findInner(json, utils::split(key, ":"));
    }

    INLINE JsonValue *JsonObjectExt::findInner(JsonObject &json, const std::vector<std::string_view> &key)
    {
        JsonObject *currentObject = &json;
        checkSegmentSize(key);
        for (int i = 0; i < key.size() - 1; ++i)
        {
            auto jsonValue = find(*currentObject, key[i]);
            if (!jsonValue || !jsonValue->is_object())
            {
                return nullptr;
            }
            currentObject = &(jsonValue->get_object());
        }
        return find(*currentObject, key.back());
    }

    INLINE const JsonValue *JsonObjectExt::findInner(const JsonObject &json, const std::vector<std::string_view> &key)
    {
        const JsonObject *currentObject = &json;
        checkSegmentSize(key);
        for (int i = 0; i < key.size() - 1; ++i)
        {
            auto jsonValue = find(*currentObject, key[i]);
            if (!jsonValue || !jsonValue->is_object())
            {
                return nullptr;
            }
            currentObject = &(jsonValue->get_object());
        }
        return find(*currentObject, key.back());
    }

    INLINE JsonValue *JsonObjectExt::insertInner(JsonObject &json, std::string_view key, JsonValue value)
    {
        return insertInner(json, utils::split(key, ":"), std::move(value));
    }

    INLINE JsonValue *JsonObjectExt::insertInner(JsonObject &json, const std::vector<std::string_view> &key,
                                                 JsonValue value)
    {
        JsonObject *currentObject = &json;
        checkSegmentSize(key);
        for (int i = 0; i < key.size() - 1; ++i)
        {
            auto jsonValue = find(*currentObject, key[i]);
            if (!jsonValue)
            {
                auto &newValue = (*currentObject)[std::string{key[i]}] = json::empty_object;
                jsonValue = &newValue;
            }
            if (!jsonValue->is_object())
            {
                throw ConfigException("Bad configuration key: '" + std::string{key[i]} + "' is not an object");
            }
            currentObject = &(jsonValue->get_object());
        }
        auto &newValue = (*currentObject)[std::string{key.back()}] = value;
        return &newValue;
    }

    INLINE void JsonObjectExt::checkSegmentSize(const std::vector<std::string_view> &key)
    {
        if (key.size() < 1)
        {
            throw ConfigException("Bad configuration key parameter, there should be at least one segment");
        }
    }
} // namespace sb::cf