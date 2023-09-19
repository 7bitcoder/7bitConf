#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/BoolDeserializer.hpp"
#include "SevenBit/Conf/Details/DoubleDeserializer.hpp"
#include "SevenBit/Conf/Details/IJsonDeserializer.hpp"
#include "SevenBit/Conf/Details/IntDeserializer.hpp"
#include "SevenBit/Conf/Details/JsonDeserializer.hpp"
#include "SevenBit/Conf/Details/NullDeserializer.hpp"
#include "SevenBit/Conf/Details/StringDeserializer.hpp"
#include "SevenBit/Conf/Details/UIntDeserializer.hpp"

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
        bool _throwOnUnknownType = false;

      public:
        JsonDeserializers(bool throwOnUnknownType);

        const IJsonDeserializer *getDeserializer(std::string_view type) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/JsonDeserializers.hpp"
#endif