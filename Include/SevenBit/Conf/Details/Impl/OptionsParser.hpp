#pragma once
#include <algorithm>
#include <charconv>
#include <cstdlib>
#include <optional>
#include <string_view>
#include <tao/json/from_string.hpp>
#include <vector>

#include "SevenBit/Conf/Details/Impl/Utils.hpp"
#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Details/OptionsParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"
#include "SevenBit/Conf/LibraryConfig.hpp"

namespace sb::cf
{
    INLINE OptionsParser::OptionsParser(OptionsParserConfig cfg) : _config(cfg) {}

    INLINE JsonObject OptionsParser::parseOption(std::string_view option) const
    {
        auto keyValue = utils::split(option, _config.optSplitter, 2);
        if (keyValue.size() == 1)
        {
            return parseOption(option, std::nullopt);
        }
        if (keyValue.size() == 2)
        {
            return parseOption(keyValue[0], keyValue[1]);
        }
        throw ConfigException("Wrong option format: " + std::string{option});
    }

    INLINE JsonObject OptionsParser::parseOption(std::string_view key, std::optional<std::string_view> value) const
    {
        auto keyStr = sanitizeKey(key);
        auto [keys, type] = parseKey(keyStr);
        auto jsonValue = value ? parseValue(type, *value) : getDefault(type);
        return parseOption(std::move(keys), std::move(jsonValue));
    }

    INLINE JsonObject OptionsParser::parseOption(const std::vector<std::string_view> &key, JsonValue value) const
    {
        JsonObject result{};
        JsonObjectExt::insertInner(result, key, std::move(value));
        return result;
    }

    INLINE std::string OptionsParser::sanitizeKey(std::string_view key) const
    {
        std::string str{key};
        utils::replaceAll(str, _config.alternativeKeySplitter, _config.keySplitter);
        return str;
    }

    INLINE std::pair<std::vector<std::string_view>, OptionsParser::OptionType> OptionsParser::parseKey(
        std::string_view key) const
    {
        if (utils::startsWith(key, _config.optPrefix))
        {
            key = key.substr(_config.optPrefix.size());
        }
        auto type = extractType(key);
        return {utils::split(key, _config.keySplitter), type};
    }

    INLINE OptionsParser::OptionType OptionsParser::extractType(std::string_view &key) const
    {
        auto index = key.find_last_of(_config.typeMarker);
        if (index == key.npos)
        {
            return String;
        }
        auto type = key.substr(index + 1);
        key = key.substr(0, index);
        if (utils::ignoreCaseEquals(type, "int"))
        {
            return Int;
        }
        if (utils::ignoreCaseEquals(type, "double"))
        {
            return Double;
        }
        if (utils::ignoreCaseEquals(type, "json"))
        {
            return Json;
        }
        if (utils::ignoreCaseEquals(type, "bool"))
        {
            return Bool;
        }
        if (utils::ignoreCaseEquals(type, "string"))
        {
            return String;
        }
        throw ConfigException("Wrong type format: " + std::string{type});
    }

    INLINE JsonValue OptionsParser::getDefault(OptionsParser::OptionType type) const
    {
        switch (type)
        {
        case Int:
            return 0;
        case Bool:
            return false;
        case Double:
            return 0.0;
        case Json:
            return JsonObject{};
        case String:
        default:
            return std::string{};
        }
    }

    INLINE JsonValue OptionsParser::parseValue(OptionsParser::OptionType type, std::string_view value) const
    {
        if (type == OptionType::Json)
        {
            return json::basic_from_string<JsonTraits>(value);
        }
        if (auto list = utils::split(value, _config.valueListSplitter); list.size() > 1)
        {
            JsonArray array;
            for (auto value : list)
            {
                array.push_back(parseElementValue(type, value));
            }
            return array;
        }
        return parseElementValue(type, value);
    }

    INLINE JsonValue OptionsParser::parseElementValue(OptionsParser::OptionType type, std::string_view value) const
    {
        switch (type)
        {
        case Json:
            return json::basic_from_string<JsonTraits>(value);
        case Int: {
            int res = 0;
            std::from_chars(value.data(), value.data() + value.size(), res);
            return res;
        }
        case Bool:
            return utils::ignoreCaseEquals("true", value);
        case Double:
            return std::stod(std::string{value});
        case String:
        default:
            return std::string{value};
        };
    }

} // namespace sb::cf