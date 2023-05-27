#pragma once
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Json.hpp"

namespace sb::cf
{
    EXPORT struct JsonObjectExt
    {
        static void deepMerge(JsonObject &json, const JsonObject &override);

        static JsonValue *find(JsonObject &json, std::string_view key);
        static const JsonValue *find(const JsonObject &json, std::string_view key);

        static JsonValue *findInner(JsonObject &json, std::string_view key);
        static const JsonValue *findInner(const JsonObject &json, std::string_view key);

        static JsonValue *findInner(JsonObject &json, const std::vector<std::string_view> &key);
        static const JsonValue *findInner(const JsonObject &json, const std::vector<std::string_view> &key);

        static JsonValue *insertInner(JsonObject &json, std::string_view key, JsonValue value);

        static JsonValue *insertInner(JsonObject &json, const std::vector<std::string_view> &key, JsonValue value);

      private:
        static void checkSegmentSize(const std::vector<std::string_view> &key);
    };

} // namespace sb::cf

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonObjectExt.hpp"
#endif