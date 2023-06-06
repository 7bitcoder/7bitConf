#pragma once

#include "SevenBit/Config/Details/JsonObjectExt.hpp"
#include "SevenBit/Config/Details/Utils.hpp"
#include "SevenBit/Config/Exceptions.hpp"
#include "SevenBit/Config/Json.hpp"
#include "SevenBit/Config/LibraryConfig.hpp"
#include <algorithm>
#include <charconv>
#include <cstddef>
#include <string>
#include <tao/json/type.hpp>

namespace sb::cf::details
{
    INLINE void JsonObjectExt::deepMerge(JsonValue &json, JsonValue override)
    {
        if (override.is_uninitialized()) // nullptr is mark for skip
        {
            return;
        }
        if (json.is_object() && override.is_object())
        {
            deepMerge(json.get_object(), std::move(override.get_object()));
        }
        else if (json.is_array() && override.is_array())
        {
            deepMerge(json.get_array(), std::move(override.get_array()));
        }
        else
        {
            json = std::move(override);
        }
    }

    INLINE void JsonObjectExt::deepMerge(JsonArray &json, JsonArray override)
    {
        for (size_t i = 0; i < override.size(); ++i)
        {
            if (i >= json.size())
            {
                json.push_back(JsonValue{});
            }
            deepMerge(json[i], std::move(override[i]));
        }
    }

    INLINE void JsonObjectExt::deepMerge(JsonObject &json, JsonObject override)
    {
        for (auto &[key, value] : override)
        {
            deepMerge(json[std::move(key)], std::move(value));
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

    INLINE JsonValue *JsonObjectExt::find(JsonArray &json, size_t index)
    {
        if (index < json.size())
        {
            return &json[index];
        }
        return nullptr;
    }

    INLINE const JsonValue *JsonObjectExt::find(const JsonArray &json, size_t index)
    {
        if (index < json.size())
        {
            return &json[index];
        }
        return nullptr;
    }

    INLINE JsonValue *JsonObjectExt::find(JsonArray &json, std::string_view key)
    {
        if (auto [success, index] = details::utils::tryStringTo<size_t>(key); success)
        {
            return find(json, index);
        }
        return nullptr;
    }

    INLINE const JsonValue *JsonObjectExt::find(const JsonArray &json, std::string_view key)
    {
        if (auto [success, index] = details::utils::tryStringTo<size_t>(key); success)
        {
            return find(json, index);
        }
        return nullptr;
    }

    INLINE JsonValue *JsonObjectExt::find(JsonValue &json, std::string_view key)
    {
        if (json.is_object())
        {
            return find(json.get_object(), key);
        }
        if (json.is_array())
        {
            return find(json.get_array(), key);
        }
        return nullptr;
    }

    INLINE const JsonValue *JsonObjectExt::find(const JsonValue &json, std::string_view key)
    {
        if (json.is_object())
        {
            return find(json.get_object(), key);
        }
        if (json.is_array())
        {
            return find(json.get_array(), key);
        }
        return nullptr;
    }

    INLINE JsonValue *JsonObjectExt::deepFind(JsonObject &json, std::string_view key)
    {
        return deepFind(json, details::utils::split(key, ":"));
    }

    INLINE const JsonValue *JsonObjectExt::deepFind(const JsonObject &json, std::string_view key)
    {
        return deepFind(json, details::utils::split(key, ":"));
    }

    INLINE JsonValue *JsonObjectExt::deepFind(JsonObject &json, const std::vector<std::string_view> &key)
    {
        checkSegmentSize(key);
        JsonValue *current = find(json, key[0]);
        for (size_t i = 1; i < key.size() && current; ++i)
        {
            current = find(*current, key[i]);
        }
        return current;
    }

    INLINE const JsonValue *JsonObjectExt::deepFind(const JsonObject &json, const std::vector<std::string_view> &key)
    {
        checkSegmentSize(key);
        const JsonValue *current = find(json, key[0]);
        for (size_t i = 1; i < key.size() && current; ++i)
        {
            current = find(*current, key[i]);
        }
        return current;
    }

    INLINE JsonValue &JsonObjectExt::getOrCreateInner(JsonObject &json, std::string_view key)
    {
        return getOrCreateInner(json, details::utils::split(key, ":"));
    }

    INLINE JsonValue &JsonObjectExt::getOrCreateInner(JsonObject &json, const std::vector<std::string_view> &keys)
    {
        checkSegmentSize(keys);
        JsonValue *current = &getOrCreateFromObject(json, keys[0]);
        for (size_t i = 1; i < keys.size() && current; ++i)
        {
            auto key = keys[i];
            if (!current->is_object() && !current->is_array() && !current->is_uninitialized())
            {
                throw ConfigException("Bad configuration key: '" + std::string{key} + "' is not an object or array");
            }
            auto isNumber = details::utils::isNumberString(key);
            if (current->is_uninitialized()) // mark for new one
            {
                *current = isNumber ? JsonValue(JsonArray{}) : JsonValue(JsonObject{});
            }
            if (current->is_array())
            {
                if (!isNumber)
                {
                    throw ConfigException("Bad configuration key: '" + std::string{key} +
                                          "' is not number index for array");
                }
                current = &getOrCreateFromArray(current->get_array(), details::utils::stringTo<size_t>(key));
            }
            else
            {
                current = &getOrCreateFromObject(current->get_object(), key);
            }
        }
        if (!current) // should not happen
        {
            throw NullPointnerException("object is null");
        }
        return *current;
    }

    INLINE JsonValue &JsonObjectExt::getOrCreateFromObject(JsonObject &object, std::string_view key)
    {
        auto jsonValue = find(object, key);
        if (!jsonValue)
        {
            jsonValue = &object[std::string{key}];
        }
        return *jsonValue;
    }

    INLINE JsonValue &JsonObjectExt::getOrCreateFromArray(JsonArray &array, size_t index)
    {
        auto jsonValue = find(array, index);
        if (!jsonValue)
        {
            array.resize(index + 1, JsonValue{});
        }
        return array[index];
    }

    INLINE void JsonObjectExt::checkSegmentSize(const std::vector<std::string_view> &key)
    {
        if (key.size() < 1)
        {
            throw ConfigException("Bad configuration key parameter, there should be at least one segment");
        }
    }

    INLINE void JsonObjectExt::checkKey(std::string_view key)
    {
        if (key.empty())
        {
            throw ConfigException("Key parameter cannot be empty");
        }
    }
} // namespace sb::cf::details