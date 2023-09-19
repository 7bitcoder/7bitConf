#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "SevenBit/Conf/LibraryConfig.hpp"

#include "SevenBit/Conf/Details/BoolDeserializer.hpp"
#include "SevenBit/Conf/Details/DoubleDeserializer.hpp"
#include "SevenBit/Conf/Details/ISettingDeserializer.hpp"
#include "SevenBit/Conf/Details/IntDeserializer.hpp"
#include "SevenBit/Conf/Details/JsonDeserializer.hpp"
#include "SevenBit/Conf/Details/NullDeserializer.hpp"
#include "SevenBit/Conf/Details/StringDeserializer.hpp"
#include "SevenBit/Conf/Details/UIntDeserializer.hpp"

namespace sb::cf::details
{

    class SettingDeserializers
    {
      public:
        struct IgnoreCaseEqual
        {
            bool operator()(const std::string &l, const std::string &r) const { return utils::ignoreCaseEqual(l, r); }
            bool operator()(std::string_view l, const std::string &r) const { return utils::ignoreCaseEqual(l, r); }
            bool operator()(const std::string &l, std::string_view r) const { return utils::ignoreCaseEqual(l, r); }
            bool operator()(std::string_view l, std::string_view r) const { return utils::ignoreCaseEqual(l, r); }
        };

        struct Hash
        {
            using hash_type = std::hash<std::string_view>;
            using is_transparent = void;

            std::size_t operator()(const char *str) const { return hash_type{}(str); }
            std::size_t operator()(std::string_view str) const { return hash_type{}(str); }
            std::size_t operator()(const std::string &str) const { return hash_type{}(str); }
        };

        using IgnoreCaseUnorderedMap =
            std::unordered_map<std::string, std::unique_ptr<ISettingDeserializer>, Hash, IgnoreCaseEqual>;

      private:
        IgnoreCaseUnorderedMap _deserializersMap;

      public:
        using Ptr = std::unique_ptr<SettingDeserializers>;

        static Ptr createDefault();

        IgnoreCaseUnorderedMap &getDeserializersMap();

        void add(std::string_view type, std::unique_ptr<ISettingDeserializer> deserializer);

        const ISettingDeserializer *getDeserializer(std::string_view type) const;
    };
} // namespace sb::cf::details

#ifdef _7BIT_CONF_ADD_IMPL
#include "SevenBit/Conf/Details/Impl/SettingDeserializers.hpp"
#endif