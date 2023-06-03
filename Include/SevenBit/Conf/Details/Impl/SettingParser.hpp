#pragma once
#include <charconv>

#include "SevenBit/Conf/Details/JsonObjectExt.hpp"
#include "SevenBit/Conf/Details/SettingParser.hpp"
#include "SevenBit/Conf/Details/Utils.hpp"
#include "SevenBit/Conf/Exceptions.hpp"
#include "SevenBit/Conf/Json.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(SettingParserConfig cfg) : _config(cfg) {}

    INLINE JsonObject SettingParser::parseSetting(std::string_view setting) const
    {
        auto keyValue = utils::split(setting, _config.settingSplitter, 2);
        if (keyValue.size() == 1)
        {
            return parseSetting(setting, std::nullopt);
        }
        if (keyValue.size() == 2)
        {
            return parseSetting(keyValue[0], std::make_optional(keyValue[1]));
        }
        throw SettingParserException("Wrong setting format: " + std::string{setting} +
                                     " if should follow this scheme --<name>=<value>");
    }

    INLINE JsonObject SettingParser::parseSetting(std::string_view key, std::optional<std::string_view> value) const
    {
        try
        {
            auto keyStr = sanitizeKey(key);
            auto keys = parseKey(keyStr);
            auto type = extractType(keys.back());
            return parseSetting(std::move(keys), parseValue(type, value));
        }
        catch (std::exception &e)
        {
            throw SettingParserException{"Error for setting: '" + std::string{key} + "', value: '" +
                                         std::string{value ? *value : ""} + "' error: " + e.what()};
        }
    }

    INLINE JsonObject SettingParser::parseSetting(const std::vector<std::string_view> &key, JsonValue value) const
    {
        JsonObject result{};
        JsonObjectExt::getOrCreateInner(result, key) = std::move(value);
        return result;
    }

    INLINE std::string SettingParser::sanitizeKey(std::string_view key) const
    {
        std::string str{key};
        utils::replaceAll(str, _config.alternativeKeySplitter, _config.keySplitter);
        return str;
    }

    INLINE std::vector<std::string_view> SettingParser::parseKey(std::string_view key) const
    {
        if (utils::startsWith(key, _config.settingPrefix))
        {
            key = key.substr(_config.settingPrefix.size());
        }
        if (key.empty())
        {
            throw SettingParserException{"Key canot be empty"};
        }
        return utils::split(key, _config.keySplitter);
    }

    INLINE SettingParser::SettingType SettingParser::extractType(std::string_view &key) const
    {
        auto index = key.find_last_of(_config.typeMarker);
        if (index == key.npos)
        {
            return String;
        }
        auto type = key.substr(index + 1);
        auto originalKey = key;
        key = key.substr(0, index);
        if (utils::ignoreCaseEquals(type, "bool"))
        {
            return Bool;
        }
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
        if (utils::ignoreCaseEquals(type, "string"))
        {
            return String;
        }
        // restore key and assume its string
        key = originalKey;
        return String;
    }

    INLINE JsonValue SettingParser::parseValue(SettingParser::SettingType type,
                                               std::optional<std::string_view> value) const
    {
        switch (type)
        {
        case Json:
            return value ? json::basic_from_string<JsonTraits>(*value) : JsonValue(JsonObject{});
        case Int:
            return value ? std::stoi(std::string{*value}) : 0;
        case Bool:
            return value ? utils::ignoreCaseEquals("true", *value) : false;
        case Double:
            return value ? std::stod(std::string{*value}) : 0.0;
        case String:
        default:
            return std::string{value ? *value : ""};
        };
    }
} // namespace sb::cf::details