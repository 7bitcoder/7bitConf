#pragma once

#include <tao/json.hpp>

#include "SevenBit/Config/LibraryConfig.hpp"

namespace sb::cf
{
    namespace json = tao::json;

    template <class T> struct JsonTraits : public json::traits<T>
    {
    };

    using JsonValue = json::basic_value<JsonTraits>;

    using JsonObject = JsonValue::object_t;
    using JsonArray = JsonValue::array_t;
} // namespace sb::cf