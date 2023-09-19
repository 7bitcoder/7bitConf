#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/Details/JsonDeserializers.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"

namespace sb::cf::details
{
    INLINE JsonDeserializers::JsonDeserializers(bool throwOnUnknownType) : _throwOnUnknownType(throwOnUnknownType) {}

    INLINE const IJsonDeserializer *JsonDeserializers::getDeserializer(std::string_view type) const
    {
        if (utils::ignoreCaseEqual(type, "string"))
        {
            return &_string;
        }
        if (utils::ignoreCaseEqual(type, "bool"))
        {
            return &_bool;
        }
        if (utils::ignoreCaseEqual(type, "int"))
        {
            return &_int;
        }
        if (utils::ignoreCaseEqual(type, "double"))
        {
            return &_double;
        }
        if (utils::ignoreCaseEqual(type, "json"))
        {
            return &_json;
        }
        if (utils::ignoreCaseEqual(type, "uint"))
        {
            return &_uint;
        }
        if (utils::ignoreCaseEqual(type, "null"))
        {
            return &_null;
        }
        if (_throwOnUnknownType)
        {
            // todo throw
        }
        return _
    }
} // namespace sb::cf::details