#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/Details/IJsonDeserializer.hpp"

namespace sb::cf::details
{
    class JsonDeserializers
    {
      private:
        StringDeserializer _string;
        BoolDeserializer _bool;
        IntDeserializer _int;
        DoubleDeserializer _double;
        JsonDeserializer _json;
        UIntDeserializer _uint;
        NullDeserializer _null;

      public:
        const IJsonDeserializer &getDeserializer(std::string_view type) const
        {
            if (utils::ignoreCaseEqual(type, "string"))
            {
                return _string;
            }
            if (utils::ignoreCaseEqual(type, "bool"))
            {
                return _bool;
            }
            if (utils::ignoreCaseEqual(type, "int"))
            {
                return _int;
            }
            if (utils::ignoreCaseEqual(type, "double"))
            {
                return _double;
            }
            if (utils::ignoreCaseEqual(type, "json"))
            {
                return _json;
            }
            if (utils::ignoreCaseEqual(type, "uint"))
            {
                return _uint;
            }
            if (utils::ignoreCaseEqual(type, "null"))
            {
                return _null;
            }
            return _string;
        }
    };
} // namespace sb::cf::details