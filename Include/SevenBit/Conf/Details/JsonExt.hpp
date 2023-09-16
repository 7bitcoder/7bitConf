#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/Setting.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    struct EXPORT JsonExt
    {
        static JsonValue *find(JsonValue &json, std::string_view key);
        static const JsonValue *find(const JsonValue &json, std::string_view key);

        static JsonValue *find(JsonObject &json, std::string_view key);
        static const JsonValue *find(const JsonObject &json, std::string_view key);

        static JsonValue *find(JsonArray &json, size_t index);
        static const JsonValue *find(const JsonArray &json, size_t index);

        static JsonValue *find(JsonArray &json, std::string_view key);
        static const JsonValue *find(const JsonArray &json, std::string_view key);

        static JsonValue *deepFind(JsonValue &json, std::string_view key);
        static const JsonValue *deepFind(const JsonValue &json, std::string_view key);

        static JsonValue *deepFind(JsonValue &json, const std::vector<std::string_view> &key);
        static const JsonValue *deepFind(const JsonValue &json, const std::vector<std::string_view> &key);

        static JsonValue *deepFind(JsonObject &json, std::string_view key);
        static const JsonValue *deepFind(const JsonObject &json, std::string_view key);

        static JsonValue *deepFind(JsonObject &json, const std::vector<std::string_view> &key);
        static const JsonValue *deepFind(const JsonObject &json, const std::vector<std::string_view> &key);

        static JsonValue *deepFind(JsonArray &json, std::string_view key);
        static const JsonValue *deepFind(const JsonArray &json, std::string_view key);

        static JsonValue *deepFind(JsonArray &json, const std::vector<std::string_view> &key);
        static const JsonValue *deepFind(const JsonArray &json, const std::vector<std::string_view> &key);

        static JsonValue &getOrOverride(JsonValue &json, std::string_view key);

        static JsonValue &getOrOverride(JsonObject &json, std::string_view key);

        static JsonValue &getOrOverride(JsonArray &array, size_t index);

        static JsonValue &deepGetOrOverride(JsonObject &json, std::string_view key);

        static JsonValue &deepGetOrOverride(JsonObject &json, const std::vector<std::string_view> &keys);

        static void deepMerge(JsonValue &json, JsonValue override);

        static void deepMerge(JsonArray &json, JsonArray override);

        static void deepMerge(JsonObject &json, JsonObject override);

        static void updateWithSetting(JsonObject &json, Setting &setting);

        static void checkSegmentSize(const std::vector<std::string_view> &key);
        static void checkKey(std::string_view key);
    };

} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonObjectExt.hpp"
#endif