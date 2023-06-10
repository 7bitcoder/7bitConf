#pragma once
#include <cstddef>
#include <cstdint>
#include <tao/json/from_string.hpp>

#include "SevenBit/Config/Details/JsonObjectExt.hpp"
#include "SevenBit/Config/Details/SettingParser.hpp"
#include "SevenBit/Config/Details/Utils.hpp"
#include "SevenBit/Config/Exceptions.hpp"
#include "SevenBit/Config/Json.hpp"

namespace sb::cf::details
{
    INLINE SettingParser::SettingParser(SettingParserConfig cfg) : _config(cfg) {}

    INLINE JsonObject SettingParser::parseSetting(std::string_view setting) const
    {
        auto keyValue = details::utils::split(setting, _config.settingSplitter, 2);
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
            auto type = extractType(key);
            auto keyStr = sanitizeKey(key);
            auto keys = parseKey(keyStr);
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
        details::utils::replaceAll(str, _config.alternativeKeySplitter, _config.keySplitter);
        return str;
    }

    INLINE std::vector<std::string_view> SettingParser::parseKey(std::string_view key) const
    {
        if (details::utils::startsWith(key, _config.settingPrefix))
        {
            key = key.substr(_config.settingPrefix.size());
        }
        if (key.empty())
        {
            throw SettingParserException{"Key canot be empty"};
        }
        return details::utils::split(key, _config.keySplitter);
    }

    INLINE SettingParser::SettingType SettingParser::extractType(std::string_view &key) const
    {
        if (tryExtractType(key, "bool"))
        {
            return Bool;
        }
        if (tryExtractType(key, "int"))
        {
            return Int;
        }
        if (tryExtractType(key, "double"))
        {
            return Double;
        }
        if (tryExtractType(key, "json"))
        {
            return Json;
        }
        if (tryExtractType(key, "string"))
        {
            return String;
        }
        if (tryExtractType(key, "uint"))
        {
            return UInt;
        }
        if (tryExtractType(key, "null"))
        {
            return Null;
        }
        return String;
    }

    INLINE bool SettingParser::tryExtractType(std::string_view &value, std::string_view typeStr) const
    {
        if (details::utils::endsWith(value, typeStr, true))
        {
            auto mutated = value;
            mutated.remove_suffix(typeStr.size());
            if (details::utils::endsWith(mutated, _config.typeMarker))
            {
                mutated.remove_suffix(_config.typeMarker.size());
                value = mutated;
                return true;
            }
            if (details::utils::endsWith(mutated, _config.alternativeTypeMarker))
            {
                mutated.remove_suffix(_config.alternativeTypeMarker.size());
                value = mutated;
                return true;
            }
        }
        return false;
    }

    INLINE JsonValue SettingParser::parseValue(SettingParser::SettingType type,
                                               std::optional<std::string_view> value) const
    {
        switch (type)
        {
        case Json:
            return value ? json::basic_from_string<JsonTraits>(*value) : JsonValue();
        case Int:
            return value ? details::utils::stringTo<std::int64_t>(*value) : 0;
        case UInt:
            return value ? details::utils::stringTo<std::uint64_t>(*value) : 0;
        case Bool:
            return value ? details::utils::stringTo<bool>(*value) : false;
        case Double:
            return value ? details::utils::stringTo<double>(*value) : 0.0;
        case Null:
            return json::null;
        case String:
        default:
            return std::string{value ? *value : ""};
        };
    }
} // namespace sb::cf::details