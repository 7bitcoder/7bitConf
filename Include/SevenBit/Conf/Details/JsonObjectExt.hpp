#pragma once
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    EXPORT struct JsonObjectExt
    {
        static void deepMerge(JsonValue &json, JsonValue override);
        static void deepMerge(JsonArray &json, JsonArray override);
        static void deepMerge(JsonObject &json, JsonObject override);

        static JsonValue *find(JsonValue &json, std::string_view key);
        static const JsonValue *find(const JsonValue &json, std::string_view key);

        static JsonValue *find(JsonArray &json, size_t index);
        static const JsonValue *find(const JsonArray &json, size_t index);

        static JsonValue *find(JsonArray &json, std::string_view index);
        static const JsonValue *find(const JsonArray &json, std::string_view index);

        static JsonValue *find(JsonObject &json, std::string_view key);
        static const JsonValue *find(const JsonObject &json, std::string_view key);

        static JsonValue *findInner(JsonObject &json, std::string_view key);
        static const JsonValue *findInner(const JsonObject &json, std::string_view key);

        static JsonValue *findInner(JsonObject &json, const std::vector<std::string_view> &key);
        static const JsonValue *findInner(const JsonObject &json, const std::vector<std::string_view> &key);

        static JsonValue &getOrCreateInner(JsonObject &json, std::string_view key);

        static JsonValue &getOrCreateInner(JsonObject &json, const std::vector<std::string_view> &key);

      private:
        static JsonValue &getOrCreateFromObject(JsonObject &json, std::string_view key);
        static JsonValue &getOrCreateFromArray(JsonArray &json, size_t index);

        static void checkSegmentSize(const std::vector<std::string_view> &key);
        static void checkKey(std::string_view key);
    };

} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonObjectExt.hpp"
#endif