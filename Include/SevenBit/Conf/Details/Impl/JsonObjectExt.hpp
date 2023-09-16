#pragma once
#include <algorithm>
#include <cstddef>
#include <string>
#include <tao/json/type.hpp>

#include "SevenBit/Conf/Details/JsonExt.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    INLINE JsonValue *JsonExt::find(JsonValue &json, std::string_view key)
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

    INLINE const JsonValue *JsonExt::find(const JsonValue &json, std::string_view key)
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

    INLINE JsonValue *JsonExt::find(JsonObject &json, std::string_view key)
    {
        if (auto it = json.find(key); it != json.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    INLINE const JsonValue *JsonExt::find(const JsonObject &json, std::string_view key)
    {
        if (auto it = json.find(key); it != json.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    INLINE JsonValue *JsonExt::find(JsonArray &json, size_t index)
    {
        if (index < json.size())
        {
            return &json[index];
        }
        return nullptr;
    }

    INLINE const JsonValue *JsonExt::find(const JsonArray &json, size_t index)
    {
        if (index < json.size())
        {
            return &json[index];
        }
        return nullptr;
    }

    INLINE JsonValue *JsonExt::find(JsonArray &json, std::string_view key)
    {
        if (auto [success, index] = details::utils::tryStringTo<size_t>(key); success)
        {
            return find(json, index);
        }
        return nullptr;
    }

    INLINE const JsonValue *JsonExt::find(const JsonArray &json, std::string_view key)
    {
        if (auto [success, index] = details::utils::tryStringTo<size_t>(key); success)
        {
            return find(json, index);
        }
        return nullptr;
    }

    template <class TJson> auto deepFindImpl(TJson &json, const std::vector<std::string_view> &key)
    {
        JsonExt::checkSegmentSize(key);
        auto current = JsonExt::find(json, key[0]);
        for (size_t i = 1; i < key.size() && current; ++i)
        {
            current = JsonExt::find(*current, key[i]);
        }
        return current;
    }

    INLINE JsonValue *JsonExt::deepFind(JsonValue &json, std::string_view key)
    {
        return deepFind(json, details::utils::split(key, ":"));
    }

    INLINE const JsonValue *JsonExt::deepFind(const JsonValue &json, std::string_view key)
    {
        return deepFind(json, details::utils::split(key, ":"));
    }

    INLINE JsonValue *JsonExt::deepFind(JsonValue &json, const std::vector<std::string_view> &key)
    {
        return deepFindImpl(json, key);
    }

    INLINE const JsonValue *JsonExt::deepFind(const JsonValue &json, const std::vector<std::string_view> &key)
    {
        return deepFindImpl(json, key);
    }

    INLINE JsonValue *JsonExt::deepFind(JsonObject &json, std::string_view key)
    {
        return deepFind(json, details::utils::split(key, ":"));
    }

    INLINE const JsonValue *JsonExt::deepFind(const JsonObject &json, std::string_view key)
    {
        return deepFind(json, details::utils::split(key, ":"));
    }

    INLINE JsonValue *JsonExt::deepFind(JsonObject &json, const std::vector<std::string_view> &key)
    {
        return deepFindImpl(json, key);
    }

    INLINE const JsonValue *JsonExt::deepFind(const JsonObject &json, const std::vector<std::string_view> &key)
    {
        return deepFindImpl(json, key);
    }

    INLINE JsonValue *JsonExt::deepFind(JsonArray &json, std::string_view key)
    {
        return deepFind(json, details::utils::split(key, ":"));
    }

    INLINE const JsonValue *JsonExt::deepFind(const JsonArray &json, std::string_view key)
    {
        return deepFind(json, details::utils::split(key, ":"));
    }

    INLINE JsonValue *JsonExt::deepFind(JsonArray &json, const std::vector<std::string_view> &key)
    {
        return deepFindImpl(json, key);
    }

    INLINE const JsonValue *JsonExt::deepFind(const JsonArray &json, const std::vector<std::string_view> &key)
    {
        return deepFindImpl(json, key);
    }

    INLINE JsonValue &JsonExt::getOrOverride(JsonValue &json, std::string_view key)
    {
        auto isNumber = utils::isNumberString(key);
        if (!json.is_object() && !json.is_array())
        {
            json = isNumber ? JsonValue(JsonArray{}) : JsonValue(JsonObject{});
        }
        if (json.is_array())
        {
            if (isNumber)
            {
                return getOrOverride(json.get_array(), utils::stringTo<size_t>(key));
            }
            json.set_object({});
        }
        return getOrOverride(json.get_object(), key);
    }

    INLINE JsonValue &JsonExt::getOrOverride(JsonObject &json, std::string_view key)
    {
        auto jsonValue = find(json, key);
        if (!jsonValue)
        {
            jsonValue = &json[std::string{key}];
        }
        return *jsonValue;
    }

    INLINE JsonValue &JsonExt::getOrOverride(JsonArray &array, size_t index)
    {
        auto jsonValue = find(array, index);
        if (!jsonValue)
        {
            array.resize(index + 1, JsonValue{});
        }
        return array[index];
    }

    INLINE JsonValue &JsonExt::deepGetOrOverride(JsonObject &json, std::string_view key)
    {
        return deepGetOrOverride(json, details::utils::split(key, ":"));
    }

    INLINE JsonValue &JsonExt::deepGetOrOverride(JsonObject &json, const std::vector<std::string_view> &keys)
    {
        checkSegmentSize(keys);
        JsonValue *current = &getOrOverride(json, keys[0]);
        for (size_t i = 1; i < keys.size() && current; ++i)
        {
            current = &getOrOverride(*current, keys[i]);
        }
        if (!current) // should not happen
        {
            throw NullPointnerException("object is null");
        }
        return *current;
    }

    INLINE void JsonExt::deepMerge(JsonValue &json, JsonValue override)
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

    INLINE void JsonExt::deepMerge(JsonArray &json, JsonArray override)
    {
        for (size_t i = 0; i < override.size(); ++i)
        {
            if (i >= json.size())
            {
                json.emplace_back();
            }
            deepMerge(json[i], std::move(override[i]));
        }
    }

    INLINE void JsonExt::deepMerge(JsonObject &json, JsonObject override)
    {
        for (auto &[key, value] : override)
        {
            deepMerge(json[key], std::move(value));
        }
    }

    INLINE void JsonExt::updateWithSetting(JsonObject &json, Setting setting)
    {
        deepGetOrOverride(json, setting.getKey()) = std::move(setting.getValue());
    }

    INLINE void JsonExt::checkSegmentSize(const std::vector<std::string_view> &key)
    {
        if (key.empty())
        {
            throw ConfigException("Bad configuration key parameter, there should be at least one segment");
        }
    }

    INLINE void JsonExt::checkKey(std::string_view key)
    {
        if (key.empty())
        {
            throw ConfigException("Key parameter cannot be empty");
        }
    }
} // namespace sb::cf::details